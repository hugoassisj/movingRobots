/**
 * @file backendcontroller.cpp
 * @brief Implementation of BackendController: the C++/QML bridge.
 *
 * This file contains:
 *   - Constructor / destructor (thread lifecycle management)
 *   - Property accessors and setters (QML binding support)
 *   - Q_INVOKABLE action handlers (manual buffer removal, debug output,
 *     keyboard robot movement)
 *   - Static worker functions (source and processor thread entry points)
 *
 * Threading summary:
 *   - Constructor and destructor run on the GUI thread.
 *   - Property accessors/setters run on the GUI thread (called by QML).
 *   - sourceWorker and processorWorker run on their respective std::threads.
 *   - All cross-thread data access goes through thread-safe interfaces.
 */

#include "backendcontroller.h"

#include <algorithm>
#include <iostream>
#include <chrono>
#include <random>
#include <cmath>

// ============================================================================
// Construction / Destruction
// ============================================================================

BackendController::BackendController(QObject* parent)
    : QObject(parent)
    , robotPtrs_{&robot0_, &robot1_, &robot2_}
    , sourcePtrs_{&source0_, &source1_, &source2_}
{
    // --- Buffer polling timer (50 ms) ---
    // Buffer is not a QObject, so we poll its size periodically.
    // The timer fires on the GUI thread, making it safe to emit
    // bufferChanged() without cross-thread signal routing.
    connect(&bufferTimer_, &QTimer::timeout, this, [this]() {
        int size = buffer_.getSize();
        if (size != lastBufferSize_) {
            lastBufferSize_ = size;
            emit bufferChanged();   // Triggers QML ProgressBar re-evaluation.
        }
    });
    bufferTimer_.start(BUFFER_POLL_MS);

    // --- Launch source threads ---
    // Each source thread owns one Source instance exclusively.
    // Shared state (robots, buffer, config) is passed by reference;
    // all are thread-safe internally.
    for (int i = 0; i < NUM_ROBOTS; ++i) {
        sourceThreads_[i] = std::thread(
            sourceWorker, i, std::ref(*sourcePtrs_[i]),
            std::ref(robotPtrs_), std::ref(buffer_), std::ref(config_));
    }

    // --- Launch processor thread ---
    // Single consumer thread that drains the buffer and updates robots.
    processorThread_ = std::thread(
        processorWorker,
        std::ref(processor_), std::ref(robotPtrs_),
        std::ref(buffer_), std::ref(config_));

    std::cout << "[BackendController] All worker threads started." << std::endl;
}

BackendController::~BackendController()
{
    std::cout << "[BackendController] Shutting down worker threads..." << std::endl;

    // Stop the buffer-polling timer first so it doesn't fire while we
    // tear down worker threads and domain objects.
    bufferTimer_.stop();

    // Signal all workers to exit their main loop.
    // Workers check this flag each iteration; the tryPut/tryTake timeouts
    // (100 ms) ensure they wake up promptly even if blocked on the buffer.
    config_.running.store(false);

    // Join all threads to ensure clean shutdown before destroying
    // domain objects. Without joining, the workers might access
    // destroyed Buffer/Processor/Robot instances.
    for (auto& thread : sourceThreads_) {
        if (thread.joinable()) thread.join();
    }
    if (processorThread_.joinable()) processorThread_.join();

    std::cout << "[BackendController] All threads stopped." << std::endl;
}

// ============================================================================
// Property Accessors
// ============================================================================
// All accessors run on the GUI thread. Atomic loads are used for config
// values to read the latest value written by QML property setters.
// Buffer accessors delegate to Buffer's mutex-protected getSize/getMaxSize.

QList<Robot*> BackendController::robots() const
{
    // robotPtrs_ elements are Robot* (non-const); accessing them through a
    // const method yields Robot*const, which is implicitly convertible back
    // to Robot*.  No const_cast needed.
    return {robotPtrs_.begin(), robotPtrs_.end()};
}

int BackendController::bufferSize() const
{
    return buffer_.getSize();
}

int BackendController::bufferMaxSize() const
{
    return buffer_.getMaxSize();
}

bool BackendController::source0Enabled() const { return config_.sourceEnabled[0].load(); }
bool BackendController::source1Enabled() const { return config_.sourceEnabled[1].load(); }
bool BackendController::source2Enabled() const { return config_.sourceEnabled[2].load(); }
bool BackendController::processorEnabled() const { return config_.processorEnabled.load(); }

int BackendController::source0Delay() const { return config_.sourceDelay[0].load(); }
int BackendController::source1Delay() const { return config_.sourceDelay[1].load(); }
int BackendController::source2Delay() const { return config_.sourceDelay[2].load(); }
int BackendController::processorDelay() const { return config_.processorDelay.load(); }

int BackendController::selectedRobot() const { return selectedRobot_; }

int BackendController::roomWidth()  const { return Constants::ROOM_WIDTH; }
int BackendController::roomHeight() const { return Constants::ROOM_HEIGHT; }
int BackendController::gridStep()   const { return Constants::GRID_STEP; }

// ============================================================================
// Property Setters
// ============================================================================
// All setters run on the GUI thread. They perform an atomic compare-and-store
// to update the shared config, then emit a change signal only if the value
// actually changed. This prevents redundant QML binding evaluations.
//
// The atomic store is immediately visible to worker threads (sequentially
// consistent ordering by default). Workers will pick up the new value on
// their next loop iteration.

void BackendController::setSource0Enabled(bool enabled)
{
    if (config_.sourceEnabled[0].load() != enabled) {
        config_.sourceEnabled[0].store(enabled);
        emit source0EnabledChanged();
    }
}

void BackendController::setSource1Enabled(bool enabled)
{
    if (config_.sourceEnabled[1].load() != enabled) {
        config_.sourceEnabled[1].store(enabled);
        emit source1EnabledChanged();
    }
}

void BackendController::setSource2Enabled(bool enabled)
{
    if (config_.sourceEnabled[2].load() != enabled) {
        config_.sourceEnabled[2].store(enabled);
        emit source2EnabledChanged();
    }
}

void BackendController::setProcessorEnabled(bool enabled)
{
    if (config_.processorEnabled.load() != enabled) {
        config_.processorEnabled.store(enabled);
        emit processorEnabledChanged();
    }
}

void BackendController::setSource0Delay(int delay)
{
    if (config_.sourceDelay[0].load() != delay) {
        config_.sourceDelay[0].store(delay);
        emit source0DelayChanged();
    }
}

void BackendController::setSource1Delay(int delay)
{
    if (config_.sourceDelay[1].load() != delay) {
        config_.sourceDelay[1].store(delay);
        emit source1DelayChanged();
    }
}

void BackendController::setSource2Delay(int delay)
{
    if (config_.sourceDelay[2].load() != delay) {
        config_.sourceDelay[2].store(delay);
        emit source2DelayChanged();
    }
}

void BackendController::setProcessorDelay(int delay)
{
    if (config_.processorDelay.load() != delay) {
        config_.processorDelay.store(delay);
        emit processorDelayChanged();
    }
}

void BackendController::setSelectedRobot(int index)
{
    if (index >= 0 && index < NUM_ROBOTS && selectedRobot_ != index) {
        selectedRobot_ = index;
        emit selectedRobotChanged();
    }
}

// ============================================================================
// QML-Invokable Actions
// ============================================================================
// These methods are called from the GUI thread in response to user interaction.
// They access thread-safe domain objects (Buffer, Processor, Robot) without
// additional locking.

void BackendController::manualRemove()
{
    Vector2D position;
    // Non-blocking take (timeout = 0): if the buffer is empty, return
    // immediately without freezing the GUI.
    if (buffer_.tryTake(position, 0)) {
        Vector2D meanPosition;
        // Feed the taken item through the same processing pipeline
        // that the processor worker thread uses.
        if (processor_.insertAndProcess(position, meanPosition)) {
            if (meanPosition.robotId >= 0 && meanPosition.robotId < NUM_ROBOTS) {
                // Atomic read-clamp-write prevents TOCTOU races with
                // the processor worker thread doing the same operation.
                robotPtrs_[meanPosition.robotId]->clampedSetPosition(
                    meanPosition, Constants::GRID_STEP,
                    Constants::ROOM_WIDTH, Constants::ROOM_HEIGHT);
            }
        }
        std::cout << processor_.toString();
        emit bufferChanged();  // Immediate UI refresh (no need to wait for timer).
    }
}

void BackendController::printState()
{
    std::cout << buffer_.toString();
    std::cout << processor_.toString();
}

void BackendController::moveSelectedRobot(int dx, int dy)
{
    if (selectedRobot_ < 0 || selectedRobot_ >= NUM_ROBOTS) return;

    // Clamp so the robot can only jump to an adjacent cell (one grid step).
    // This prevents injecting large movements that could skip cells.
    dx = std::clamp(dx, -KEY_MOVEMENT, KEY_MOVEMENT);
    dy = std::clamp(dy, -KEY_MOVEMENT, KEY_MOVEMENT);

    // Read current position (mutex-protected inside Robot).
    Vector2D position = robotPtrs_[selectedRobot_]->getPosition();

    // Keyboard movement wraps around (toroidal) using modular arithmetic.
    // The double-modulo pattern "((a % m) + m) % m" handles negative values
    // correctly in C++ (where % can return negative for negative operands).
    constexpr int W = Constants::ROOM_WIDTH;
    constexpr int H = Constants::ROOM_HEIGHT;
    position.x = ((position.x + dx) % W + W) % W;
    position.y = ((position.y + dy) % H + H) % H;

    // Write new position (mutex-protected, direct signal emission on GUI thread).
    robotPtrs_[selectedRobot_]->setPosition(position);
}

// ============================================================================
// Worker Thread Functions
// ============================================================================
// These static functions are the entry points for std::thread. They receive
// all shared state by reference: no `this` pointer is involved. This makes
// the threading model explicit and avoids accidental access to non-thread-safe
// BackendController members.

void BackendController::sourceWorker(
    int sourceId,
    Source& source,
    std::array<Robot*, NUM_ROBOTS>& robots,
    Buffer& buffer,
    ThreadConfig& config)
{
    std::cout << "[Source " << sourceId << "] Started." << std::endl;

    // Per-thread RNG for jitter. Using a thread-local RNG avoids shared
    // state and the need for locking.
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> jitter(0, SOURCE_JITTER_MS);

    while (config.running.load()) {
        if (config.sourceEnabled[sourceId].load()) {
            // Each cycle produces one reading for every robot.
            for (int robotIndex = 0; robotIndex < NUM_ROBOTS; ++robotIndex) {
                // Check shutdown flag between robots to exit promptly.
                if (!config.running.load()) return;

                // source.produce() reads the robot's current position
                // (mutex-protected) and applies a random perturbation.
                Vector2D position = source.produce(*robots[robotIndex]);

                // tryPut with 100 ms timeout: if the buffer is full, we
                // retry periodically. This avoids deadlock during shutdown:
                // if the processor is disabled and the buffer fills up,
                // blocking put() would never return.
                while (!buffer.tryPut(position, 100)) {
                    if (!config.running.load()) return;
                }

                // Small random jitter between per-robot readings adds realism
                // and reduces the chance of lock-step synchronization between
                // sources that could cause bursty buffer writes.
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(jitter(rng)));
            }
        }

        // Configurable delay between source cycles. Read atomically each
        // iteration so slider changes in QML take effect immediately.
        int delayMs = config.sourceDelay[sourceId].load();
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }

    std::cout << "[Source " << sourceId << "] Stopped." << std::endl;
}

void BackendController::processorWorker(
    Processor& processor,
    std::array<Robot*, NUM_ROBOTS>& robots,
    Buffer& buffer,
    ThreadConfig& config)
{
    std::cout << "[Processor] Started." << std::endl;

    while (config.running.load()) {
        if (config.processorEnabled.load()) {
            Vector2D position;
            // tryTake with 100 ms timeout: blocks briefly for an item,
            // then re-checks the running flag. This ensures the processor
            // wakes up within 100 ms of a shutdown request.
            if (buffer.tryTake(position, 100)) {
                Vector2D meanPosition;
                // insertAndProcess accumulates readings and returns true
                // when all sources have reported for a robot.
                if (processor.insertAndProcess(position, meanPosition)) {
                    int robotId = meanPosition.robotId;
                    if (robotId >= 0 && robotId < NUM_ROBOTS) {
                        // Atomic read-clamp-write prevents TOCTOU races
                        // with the GUI thread's manualRemove.
                        robots[robotId]->clampedSetPosition(
                            meanPosition, Constants::GRID_STEP,
                            Constants::ROOM_WIDTH, Constants::ROOM_HEIGHT);
                    }
                }
            }
        } else {
            // When processor is disabled, sleep briefly to avoid busy-waiting.
            // This is cheaper than polling tryTake repeatedly.
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        // Configurable delay between processing cycles.
        int delayMs = config.processorDelay.load();
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }

    std::cout << "[Processor] Stopped." << std::endl;
}
