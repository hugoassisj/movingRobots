/**
 * @file backendcontroller.cpp
 * @brief Implementation of BackendController — the C++/QML bridge.
 */

#include "backendcontroller.h"

#include <algorithm>
#include <iostream>
#include <chrono>
#include <random>

// ============================================================================
// Construction / Destruction
// ============================================================================

BackendController::BackendController(QObject* parent)
    : QObject(parent)
    , robotPtrs_{&robot0_, &robot1_, &robot2_}
    , sourcePtrs_{&source0_, &source1_, &source2_}
{
    // --- Buffer polling timer (50 ms) ---
    connect(&bufferTimer_, &QTimer::timeout, this, [this]() {
        int size = buffer_.getSize();
        if (size != lastBufferSize_) {
            lastBufferSize_ = size;
            emit bufferChanged();
        }
    });
    bufferTimer_.start(BUFFER_POLL_MS);

    // --- Launch source threads ---
    for (int i = 0; i < NUM_ROBOTS; ++i) {
        sourceThreads_[i] = std::thread(
            sourceWorker, i, std::ref(*sourcePtrs_[i]),
            std::ref(robotPtrs_), std::ref(buffer_), std::ref(config_));
    }

    // --- Launch processor thread ---
    processorThread_ = std::thread(
        processorWorker,
        std::ref(processor_), std::ref(robotPtrs_),
        std::ref(buffer_), std::ref(config_));

    std::cout << "[BackendController] All worker threads started." << std::endl;
}

BackendController::~BackendController()
{
    std::cout << "[BackendController] Shutting down worker threads..." << std::endl;
    config_.running.store(false);

    for (auto& thread : sourceThreads_) {
        if (thread.joinable()) thread.join();
    }
    if (processorThread_.joinable()) processorThread_.join();

    std::cout << "[BackendController] All threads stopped." << std::endl;
}

// ============================================================================
// Property Accessors
// ============================================================================

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

void BackendController::manualRemove()
{
    Vector2D position;
    if (buffer_.tryTake(position, 0)) {
        Vector2D meanPosition;
        if (processor_.insertAndProcess(position, meanPosition)) {
            if (meanPosition.robotId >= 0 && meanPosition.robotId < NUM_ROBOTS) {
                robotPtrs_[meanPosition.robotId]->setPosition(meanPosition);
            }
        }
        std::cout << processor_.toString();
        emit bufferChanged();
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
    dx = std::clamp(dx, -KEY_MOVEMENT, KEY_MOVEMENT);
    dy = std::clamp(dy, -KEY_MOVEMENT, KEY_MOVEMENT);

    Vector2D position = robotPtrs_[selectedRobot_]->getPosition();

    // Keyboard movement wraps around (toroidal) using modular arithmetic.
    constexpr int W = Constants::ROOM_WIDTH;
    constexpr int H = Constants::ROOM_HEIGHT;
    position.x = ((position.x + dx) % W + W) % W;
    position.y = ((position.y + dy) % H + H) % H;

    robotPtrs_[selectedRobot_]->setPosition(position);
}

// ============================================================================
// Worker Thread Functions
// ============================================================================

void BackendController::sourceWorker(
    int sourceId,
    Source& source,
    std::array<Robot*, NUM_ROBOTS>& robots,
    Buffer& buffer,
    ThreadConfig& config)
{
    std::cout << "[Source " << sourceId << "] Started." << std::endl;

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> jitter(0, SOURCE_JITTER_MS);

    while (config.running.load()) {
        if (config.sourceEnabled[sourceId].load()) {
            for (int robotIndex = 0; robotIndex < NUM_ROBOTS; ++robotIndex) {
                if (!config.running.load()) return;

                Vector2D position = source.produce(*robots[robotIndex]);

                while (!buffer.tryPut(position, 100)) {
                    if (!config.running.load()) return;
                }

                std::this_thread::sleep_for(
                    std::chrono::milliseconds(jitter(rng)));
            }
        }

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
            if (buffer.tryTake(position, 100)) {
                Vector2D meanPosition;
                if (processor.insertAndProcess(position, meanPosition)) {
                    int robotId = meanPosition.robotId;
                    if (robotId >= 0 && robotId < NUM_ROBOTS) {
                        robots[robotId]->setPosition(meanPosition);
                    }
                }
            }
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        int delayMs = config.processorDelay.load();
        std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    }

    std::cout << "[Processor] Stopped." << std::endl;
}
