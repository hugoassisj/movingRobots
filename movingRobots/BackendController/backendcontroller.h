#ifndef BACKENDCONTROLLER_H
#define BACKENDCONTROLLER_H

#include <QObject>
#include <QTimer>
#include <array>
#include <thread>
#include <atomic>

#include "../Robot/robot.h"
#include "../Buffer/buffer.h"
#include "../Source/source.h"
#include "../Processor/processor.h"
#include "../Vector2D/vector2d.h"
#include "../constants.h"

/**
 * @file backendcontroller.h
 * @brief Central QObject bridging C++ backend and QML UI.
 *
 * This is the top-level orchestrator of the application. It owns every
 * domain object (Robots, Sources, Buffer, Processor) and manages all
 * worker threads. It exposes a rich set of Q_PROPERTYs and Q_INVOKABLEs
 * so the QML UI can bind to and control the backend reactively.
 *
 * Threading model:
 *   - BackendController itself lives on the GUI thread.
 *   - It spawns 3 source worker threads and 1 processor worker thread.
 *   - Worker threads communicate with domain objects through thread-safe
 *     interfaces (Robot::mutex_, Buffer::semaphores + mutex_,
 *     Processor::mutex_).
 *   - Configuration changes from QML are propagated to workers via
 *     std::atomic fields in ThreadConfig (lock-free, wait-free).
 *   - Buffer size polling uses a QTimer on the GUI thread since Buffer
 *     is not a QObject and cannot emit signals.
 *
 * Lifetime: constructed in main() before exec(), destroyed when the
 * application exits. The destructor sets running = false and joins all
 * worker threads, ensuring a clean shutdown.
 */

/**
 * @brief Atomic configuration shared between the GUI and worker threads.
 *
 * All fields are std::atomic so they can be read/written from any thread
 * without additional locking. The GUI thread writes these values in
 * response to QML property changes; worker threads read them each
 * iteration of their main loop.
 *
 * Using atomics here instead of a mutex avoids blocking the GUI thread
 * when workers are mid-iteration, and avoids blocking workers when the
 * GUI updates a slider. The trade-off is that workers may see a stale
 * value for at most one loop iteration, which is acceptable for
 * configuration parameters.
 */
struct ThreadConfig {
    /// Per-source enable flags. When false, the source thread sleeps instead of producing.
    std::atomic<bool> sourceEnabled[3]  = {true, true, true};

    /// Processor enable flag. When false, the processor thread sleeps instead of consuming.
    std::atomic<bool> processorEnabled{true};

    /// Per-source delay in milliseconds between production cycles.
    std::atomic<int>  sourceDelay[3]    = {100, 100, 100};

    /// Processor delay in milliseconds between consumption cycles.
    std::atomic<int>  processorDelay{20};

    /// Master shutdown flag. Set to false by the destructor to stop all workers.
    std::atomic<bool> running{true};
};

/**
 * @brief Central controller that owns all domain objects and exposes them to QML.
 *
 * Owns Robots, Sources, Buffer, Processor, and the worker threads.
 * Exposes properties, signals, and invokable methods so the QML UI can
 * bind to backend state reactively.
 *
 * A low-frequency timer polls the buffer size for the progress bar, since
 * Buffer is not a QObject and cannot emit signals directly.
 *
 * Ownership model:
 *   - Robot, Source, Buffer, Processor are value members (automatic lifetime).
 *   - Worker std::threads are joined in the destructor (RAII shutdown).
 *   - Robot pointers in robotPtrs_ are non-owning views into the value members.
 *
 * QML exposure:
 *   - Registered as a context property ("backend") in main.cpp.
 *   - QML reads robot positions, buffer state, and config via Q_PROPERTYs.
 *   - QML writes config (enable flags, delays) via property setters.
 *   - QML triggers actions via Q_INVOKABLE methods.
 */
class BackendController : public QObject {
    Q_OBJECT

    // --- Robot list for QML ---
    /// Provides the list of Robot pointers to QML for use in Repeater models.
    Q_PROPERTY(QList<Robot*> robots READ robots CONSTANT)

    // --- Buffer state ---
    /// Current number of items in the shared buffer (polled via timer).
    Q_PROPERTY(int bufferSize    READ bufferSize    NOTIFY bufferChanged)
    /// Maximum capacity of the shared buffer (constant after construction).
    Q_PROPERTY(int bufferMaxSize READ bufferMaxSize CONSTANT)

    // --- Source enable flags ---
    /// Whether source 0 (IMU) is actively producing readings.
    Q_PROPERTY(bool source0Enabled READ source0Enabled WRITE setSource0Enabled NOTIFY source0EnabledChanged)
    /// Whether source 1 (GPS) is actively producing readings.
    Q_PROPERTY(bool source1Enabled READ source1Enabled WRITE setSource1Enabled NOTIFY source1EnabledChanged)
    /// Whether source 2 (Odometry) is actively producing readings.
    Q_PROPERTY(bool source2Enabled READ source2Enabled WRITE setSource2Enabled NOTIFY source2EnabledChanged)

    // --- Processor enable flag ---
    /// Whether the processor thread is actively consuming buffer items.
    Q_PROPERTY(bool processorEnabled READ processorEnabled WRITE setProcessorEnabled NOTIFY processorEnabledChanged)

    // --- Delay values ---
    /// Delay (ms) between source 0 production cycles.
    Q_PROPERTY(int source0Delay READ source0Delay WRITE setSource0Delay NOTIFY source0DelayChanged)
    /// Delay (ms) between source 1 production cycles.
    Q_PROPERTY(int source1Delay READ source1Delay WRITE setSource1Delay NOTIFY source1DelayChanged)
    /// Delay (ms) between source 2 production cycles.
    Q_PROPERTY(int source2Delay READ source2Delay WRITE setSource2Delay NOTIFY source2DelayChanged)
    /// Delay (ms) between processor consumption cycles.
    Q_PROPERTY(int processorDelay READ processorDelay WRITE setProcessorDelay NOTIFY processorDelayChanged)

    // --- Selected robot for keyboard control ---
    /// Index of the robot currently selected for arrow-key movement.
    Q_PROPERTY(int selectedRobot READ selectedRobot WRITE setSelectedRobot NOTIFY selectedRobotChanged)

    // --- Grid constants (read-only, exposed so QML doesn't duplicate magic numbers) ---
    /// Room width in logical pixels (constant).
    Q_PROPERTY(int roomWidth  READ roomWidth  CONSTANT)
    /// Room height in logical pixels (constant).
    Q_PROPERTY(int roomHeight READ roomHeight CONSTANT)
    /// Grid cell size in logical pixels (constant).
    Q_PROPERTY(int gridStep   READ gridStep   CONSTANT)

public:
    static constexpr int NUM_ROBOTS  = Constants::NUM_ROBOTS;
    static constexpr int BUFFER_SIZE = Constants::BUFFER_SIZE;

    /**
     * @brief Constructs the controller, initializes domain objects, and launches worker threads.
     *
     * The constructor starts 3 source threads and 1 processor thread.
     * All threads run until running is set to false in the destructor.
     *
     * @param parent Optional QObject parent for Qt ownership tree.
     */
    explicit BackendController(QObject* parent = nullptr);

    /**
     * @brief Signals all worker threads to stop and waits for them to finish.
     *
     * Sets config_.running to false, which causes each worker's main loop to
     * exit. Then joins all std::threads. The "try" variants used in worker
     * loops ensure threads wake from semaphore waits within their timeout
     * period (100 ms), so shutdown completes promptly.
     */
    ~BackendController() override;

    // --- Property accessors ---
    // All accessors are called from the GUI thread by QML.
    // Atomic loads in source*/processor* accessors are wait-free.

    /** @brief Returns the list of Robot pointers for QML binding. */
    QList<Robot*> robots() const;

    /** @brief Returns the current number of items in the buffer. */
    int  bufferSize() const;
    /** @brief Returns the buffer's maximum capacity. */
    int  bufferMaxSize() const;

    /** @brief Returns whether source 0 (IMU) is enabled. */
    bool source0Enabled() const;
    /** @brief Returns whether source 1 (GPS) is enabled. */
    bool source1Enabled() const;
    /** @brief Returns whether source 2 (Odometry) is enabled. */
    bool source2Enabled() const;
    /** @brief Returns whether the processor thread is enabled. */
    bool processorEnabled() const;

    /** @brief Returns source 0 delay in milliseconds. */
    int  source0Delay() const;
    /** @brief Returns source 1 delay in milliseconds. */
    int  source1Delay() const;
    /** @brief Returns source 2 delay in milliseconds. */
    int  source2Delay() const;
    /** @brief Returns processor delay in milliseconds. */
    int  processorDelay() const;

    /** @brief Returns the index of the currently selected robot (0-2). */
    int  selectedRobot() const;

    /** @brief Returns room width constant for QML. */
    int  roomWidth() const;
    /** @brief Returns room height constant for QML. */
    int  roomHeight() const;
    /** @brief Returns grid step constant for QML. */
    int  gridStep() const;

    // --- Property setters ---
    // All setters are called from the GUI thread by QML property bindings.
    // They write to atomics and emit change signals only when the value
    // actually changes (to avoid unnecessary QML binding re-evaluations).

    /** @brief Enables/disables source 0 (IMU). Thread-safe via atomic store. */
    void setSource0Enabled(bool enabled);
    /** @brief Enables/disables source 1 (GPS). Thread-safe via atomic store. */
    void setSource1Enabled(bool enabled);
    /** @brief Enables/disables source 2 (Odometry). Thread-safe via atomic store. */
    void setSource2Enabled(bool enabled);
    /** @brief Enables/disables the processor thread. Thread-safe via atomic store. */
    void setProcessorEnabled(bool enabled);

    /** @brief Sets source 0 production delay (ms). Thread-safe via atomic store. */
    void setSource0Delay(int delay);
    /** @brief Sets source 1 production delay (ms). Thread-safe via atomic store. */
    void setSource1Delay(int delay);
    /** @brief Sets source 2 production delay (ms). Thread-safe via atomic store. */
    void setSource2Delay(int delay);
    /** @brief Sets processor consumption delay (ms). Thread-safe via atomic store. */
    void setProcessorDelay(int delay);

    /**
     * @brief Sets the index of the selected robot for keyboard control.
     * @param index Robot index (0-2). Ignored if out of range.
     */
    void setSelectedRobot(int index);

    // --- QML-invokable actions ---

    /**
     * @brief Manually removes one item from the buffer and processes it.
     *
     * Called from QML when the user clicks "Remove Item". Performs a
     * non-blocking tryTake (timeout = 0) so the GUI never freezes.
     * If an item is retrieved and processing completes a robot's row,
     * the robot position is updated immediately.
     *
     * @note Runs on the GUI thread. Thread-safe because Buffer::tryTake
     *       and Processor::insertAndProcess both use internal locking.
     */
    Q_INVOKABLE void manualRemove();

    /**
     * @brief Prints the current buffer contents and processor matrix to stdout.
     *
     * Useful for debugging. Called from QML when the user clicks "Print State".
     * @note Runs on the GUI thread.
     */
    Q_INVOKABLE void printState();

    /**
     * @brief Moves the selected robot by (dx, dy) pixels.
     *
     * Called from QML in response to arrow key presses. Movement is
     * clamped to one grid step and wraps toroidally (modular arithmetic).
     *
     * @param dx Horizontal displacement (positive = right).
     * @param dy Vertical displacement (positive = down).
     * @note Runs on the GUI thread. Robot::setPosition is mutex-protected.
     */
    Q_INVOKABLE void moveSelectedRobot(int dx, int dy);

signals:
    void bufferChanged();            ///< Buffer size changed (polled by timer).
    void source0EnabledChanged();    ///< Source 0 enable state toggled.
    void source1EnabledChanged();    ///< Source 1 enable state toggled.
    void source2EnabledChanged();    ///< Source 2 enable state toggled.
    void processorEnabledChanged();  ///< Processor enable state toggled.
    void source0DelayChanged();      ///< Source 0 delay value changed.
    void source1DelayChanged();      ///< Source 1 delay value changed.
    void source2DelayChanged();      ///< Source 2 delay value changed.
    void processorDelayChanged();    ///< Processor delay value changed.
    void selectedRobotChanged();     ///< Selected robot index changed.

private:
    // --- Domain objects ---
    // Robots are value members (not heap-allocated). Their QObject affinity
    // is the GUI thread because BackendController lives on the GUI thread.
    Robot     robot0_{0};
    Robot     robot1_{1};
    Robot     robot2_{2};
    std::array<Robot*, NUM_ROBOTS> robotPtrs_;  ///< Non-owning pointers for indexed access.

    // Sources are value members. Each is exclusively used by its worker thread.
    Source    source0_{0};
    Source    source1_{1};
    Source    source2_{2};
    std::array<Source*, NUM_ROBOTS> sourcePtrs_;  ///< Non-owning pointers for indexed access.

    Buffer    buffer_{BUFFER_SIZE};  ///< Shared bounded buffer (thread-safe internally).
    Processor processor_;            ///< Accumulator/averager (thread-safe internally).
    ThreadConfig config_;            ///< Atomic configuration read by all workers.

    // --- Worker threads ---
    // Managed as raw std::threads (not QThreads) because the workers
    // are simple loops with no need for a Qt event loop.
    std::array<std::thread, NUM_ROBOTS> sourceThreads_;  ///< One thread per source.
    std::thread processorThread_;                         ///< Single consumer thread.

    // --- UI state ---
    int selectedRobot_ = 0;  ///< Index of the robot controlled by arrow keys (GUI-thread only).

    // --- Buffer polling timer ---
    // Buffer is not a QObject and cannot emit signals. This timer polls
    // buffer size at a fixed interval and emits bufferChanged() when it
    // differs from the cached value. 50 ms polling is fast enough for
    // smooth progress bar updates without excessive overhead.
    QTimer bufferTimer_;          ///< Periodic poll for buffer size changes.
    int    lastBufferSize_ = -1;  ///< Cached value to detect changes.

    // --- Constants ---
    static constexpr int KEY_MOVEMENT       = Constants::GRID_STEP;  ///< Move exactly one grid cell per keypress.
    static constexpr int SOURCE_JITTER_MS   = 80;   ///< Maximum random jitter added between per-robot readings within a source cycle.
    static constexpr int BUFFER_POLL_MS     = 50;   ///< Interval for the buffer size polling timer.

    // --- Thread worker functions ---
    // These are static so they have no implicit `this` pointer, making
    // the threading model explicit: all shared state is passed by reference.

    /**
     * @brief Source worker thread entry point.
     *
     * Runs a loop that produces position readings for all robots using
     * the given Source, inserts them into the Buffer, and sleeps for
     * the configured delay. Exits when config.running becomes false.
     *
     * @param sourceId  Index of this source (0-2).
     * @param source    Reference to the Source instance (exclusive ownership).
     * @param robots    Array of Robot pointers (shared, read via mutex).
     * @param buffer    Reference to the shared Buffer (thread-safe).
     * @param config    Atomic configuration (shared, lock-free reads).
     */
    static void sourceWorker(int sourceId, Source& source,
                             std::array<Robot*, NUM_ROBOTS>& robots,
                             Buffer& buffer, ThreadConfig& config);

    /**
     * @brief Processor worker thread entry point.
     *
     * Runs a loop that consumes items from the Buffer, feeds them to the
     * Processor for accumulation/averaging, and updates robot positions
     * when a full set of readings has been collected. Exits when
     * config.running becomes false.
     *
     * @param processor Reference to the Processor instance (thread-safe).
     * @param robots    Array of Robot pointers (shared, written via mutex).
     * @param buffer    Reference to the shared Buffer (thread-safe).
     * @param config    Atomic configuration (shared, lock-free reads).
     */
    static void processorWorker(Processor& processor,
                                std::array<Robot*, NUM_ROBOTS>& robots,
                                Buffer& buffer, ThreadConfig& config);
};

#endif // BACKENDCONTROLLER_H
