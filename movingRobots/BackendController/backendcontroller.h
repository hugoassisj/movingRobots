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
 */

/**
 * @brief Atomic configuration shared between the GUI and worker threads.
 *
 * All fields are std::atomic so they can be read/written from any thread
 * without additional locking.
 */
struct ThreadConfig {
    std::atomic<bool> sourceEnabled[3]  = {true, true, true};
    std::atomic<bool> processorEnabled{true};
    std::atomic<int>  sourceDelay[3]    = {100, 100, 100};
    std::atomic<int>  processorDelay{20};
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
 */
class BackendController : public QObject {
    Q_OBJECT

    // --- Robot list for QML ---
    Q_PROPERTY(QList<Robot*> robots READ robots CONSTANT)

    // --- Buffer state ---
    Q_PROPERTY(int bufferSize    READ bufferSize    NOTIFY bufferChanged)
    Q_PROPERTY(int bufferMaxSize READ bufferMaxSize CONSTANT)

    // --- Source enable flags ---
    Q_PROPERTY(bool source0Enabled READ source0Enabled WRITE setSource0Enabled NOTIFY source0EnabledChanged)
    Q_PROPERTY(bool source1Enabled READ source1Enabled WRITE setSource1Enabled NOTIFY source1EnabledChanged)
    Q_PROPERTY(bool source2Enabled READ source2Enabled WRITE setSource2Enabled NOTIFY source2EnabledChanged)

    // --- Processor enable flag ---
    Q_PROPERTY(bool processorEnabled READ processorEnabled WRITE setProcessorEnabled NOTIFY processorEnabledChanged)

    // --- Delay values ---
    Q_PROPERTY(int source0Delay READ source0Delay WRITE setSource0Delay NOTIFY source0DelayChanged)
    Q_PROPERTY(int source1Delay READ source1Delay WRITE setSource1Delay NOTIFY source1DelayChanged)
    Q_PROPERTY(int source2Delay READ source2Delay WRITE setSource2Delay NOTIFY source2DelayChanged)
    Q_PROPERTY(int processorDelay READ processorDelay WRITE setProcessorDelay NOTIFY processorDelayChanged)

    // --- Selected robot for keyboard control ---
    Q_PROPERTY(int selectedRobot READ selectedRobot WRITE setSelectedRobot NOTIFY selectedRobotChanged)

    // --- Grid constants (read-only, exposed so QML doesn't duplicate magic numbers) ---
    Q_PROPERTY(int roomWidth  READ roomWidth  CONSTANT)
    Q_PROPERTY(int roomHeight READ roomHeight CONSTANT)
    Q_PROPERTY(int gridStep   READ gridStep   CONSTANT)

public:
    static constexpr int NUM_ROBOTS  = Constants::NUM_ROBOTS;
    static constexpr int BUFFER_SIZE = Constants::BUFFER_SIZE;

    explicit BackendController(QObject* parent = nullptr);
    ~BackendController() override;

    // --- Property accessors ---
    QList<Robot*> robots() const;

    int  bufferSize() const;
    int  bufferMaxSize() const;

    bool source0Enabled() const;
    bool source1Enabled() const;
    bool source2Enabled() const;
    bool processorEnabled() const;

    int  source0Delay() const;
    int  source1Delay() const;
    int  source2Delay() const;
    int  processorDelay() const;

    int  selectedRobot() const;

    int  roomWidth() const;
    int  roomHeight() const;
    int  gridStep() const;

    // --- Property setters ---
    void setSource0Enabled(bool enabled);
    void setSource1Enabled(bool enabled);
    void setSource2Enabled(bool enabled);
    void setProcessorEnabled(bool enabled);

    void setSource0Delay(int delay);
    void setSource1Delay(int delay);
    void setSource2Delay(int delay);
    void setProcessorDelay(int delay);

    void setSelectedRobot(int index);

    // --- QML-invokable actions ---
    Q_INVOKABLE void manualRemove();
    Q_INVOKABLE void printState();
    Q_INVOKABLE void moveSelectedRobot(int dx, int dy);

signals:
    void bufferChanged();
    void source0EnabledChanged();
    void source1EnabledChanged();
    void source2EnabledChanged();
    void processorEnabledChanged();
    void source0DelayChanged();
    void source1DelayChanged();
    void source2DelayChanged();
    void processorDelayChanged();
    void selectedRobotChanged();

private:
    // --- Domain objects ---
    Robot     robot0_{0};
    Robot     robot1_{1};
    Robot     robot2_{2};
    std::array<Robot*, NUM_ROBOTS> robotPtrs_;

    Source    source0_{0};
    Source    source1_{1};
    Source    source2_{2};
    std::array<Source*, NUM_ROBOTS> sourcePtrs_;

    Buffer    buffer_{BUFFER_SIZE};
    Processor processor_;
    ThreadConfig config_;

    // --- Worker threads ---
    std::array<std::thread, NUM_ROBOTS> sourceThreads_;
    std::thread processorThread_;

    // --- UI state ---
    int selectedRobot_ = 0;

    // --- Buffer polling timer ---
    QTimer bufferTimer_;
    int    lastBufferSize_ = -1;

    // --- Constants ---
    static constexpr int KEY_MOVEMENT       = Constants::GRID_STEP;  // Move exactly one grid cell per keypress.
    static constexpr int SOURCE_JITTER_MS   = 80;
    static constexpr int BUFFER_POLL_MS     = 50;

    // --- Thread worker functions ---
    static void sourceWorker(int sourceId, Source& source,
                             std::array<Robot*, NUM_ROBOTS>& robots,
                             Buffer& buffer, ThreadConfig& config);
    static void processorWorker(Processor& processor,
                                std::array<Robot*, NUM_ROBOTS>& robots,
                                Buffer& buffer, ThreadConfig& config);
};

#endif // BACKENDCONTROLLER_H
