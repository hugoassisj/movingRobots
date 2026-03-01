#ifndef ROBOT_H
#define ROBOT_H

#include <mutex>
#include <QObject>
#include "../Vector2D/vector2d.h"

/**
 * @file robot.h
 * @brief Thread-safe robot exposed to QML with notifiable properties.
 *
 * Robot is the only domain object that is directly accessed from three
 * different thread categories simultaneously:
 *   1. Source worker threads   : read position via getPosition()
 *   2. Processor worker thread : write position via setPosition()
 *   3. GUI / main thread       : read posX/posY via Q_PROPERTY bindings,
 *                                 write position via keyboard control
 *
 * All access is serialized through a std::mutex. Signal emission uses
 * Qt::QueuedConnection when called off the GUI thread to ensure safe
 * property-change notifications in QML.
 */

/**
 * @brief Represents a single robot moving within a bounded room.
 *
 * Each robot has a unique numeric ID and a 2D position. The position can
 * be read and written concurrently from multiple threads (source threads,
 * processor thread, GUI thread), so all access is protected by a mutex.
 *
 * Inherits QObject and exposes posX, posY, and robotId as Q_PROPERTYs
 * so that QML items can bind directly to robot positions.
 *
 * The initial position is generated randomly on a grid aligned to @c GRID_STEP
 * pixels within the room dimensions.
 *
 * Ownership: Robot instances are value members of BackendController and
 * live on the GUI thread (main thread affinity). They are never moved to
 * another QThread. This ensures that queued signal delivery targets the
 * correct event loop for QML property updates.
 *
 * Thread-safety:
 *   - robotId() is immutable after construction, no lock needed.
 *   - posX(), posY(), getPosition() acquire the mutex for a consistent read.
 *   - setPosition() acquires the mutex, then emits positionChanged() via
 *     QueuedConnection if called from a non-GUI thread.
 *
 * QML exposure:
 *   - posX, posY: read-only properties bound to grid visualization.
 *   - robotId: constant property used as an identifier in Repeater delegates.
 *   - The type is registered as uncreatable in main.cpp; instances are
 *     provided by BackendController::robots.
 */
class Robot : public QObject {
    Q_OBJECT

    Q_PROPERTY(int posX READ posX NOTIFY positionChanged)
    Q_PROPERTY(int posY READ posY NOTIFY positionChanged)
    Q_PROPERTY(int robotId READ robotId CONSTANT)

public:
    /**
     * @brief Constructs a robot with a given ID and a random start position.
     * @param id Unique identifier for this robot (0, 1, or 2).
     * @param parent Optional QObject parent.
     */
    explicit Robot(int id, QObject* parent = nullptr);

    /**
     * @brief Returns the robot's unique identifier.
     * @return Robot ID.
     */
    int robotId() const;

    /**
     * @brief Returns the robot's current X position.
     * @return X coordinate in pixels.
     */
    int posX() const;

    /**
     * @brief Returns the robot's current Y position.
     * @return Y coordinate in pixels.
     */
    int posY() const;

    /**
     * @brief Returns a snapshot of the robot's current position.
     * @return Copy of the current position vector.
     */
    Vector2D getPosition() const;

    /**
     * @brief Updates the robot's position (x and y only).
     *
     * Emits positionChanged() if the position actually changed.
     * Safe to call from any thread: the signal is emitted via
     * QMetaObject::invokeMethod with Qt::QueuedConnection when
     * called from a non-GUI thread.
     *
     * @param newPosition New coordinates to apply.
     */
    void setPosition(const Vector2D& newPosition);

    /**
     * @brief Atomically reads current position, clamps movement, and updates.
     *
     * Performs the entire read-modify-write under a single mutex lock
     * to prevent TOCTOU races between concurrent callers (e.g., processor
     * worker thread and GUI manual-remove).
     *
     * @param target     Desired new position.
     * @param gridStep   Maximum allowed movement per axis.
     * @param roomWidth  Room width for boundary clamping.
     * @param roomHeight Room height for boundary clamping.
     */
    void clampedSetPosition(const Vector2D& target, int gridStep,
                            int roomWidth, int roomHeight);

signals:
    /**
     * @brief Emitted when the robot's position changes.
     */
    void positionChanged();

private:
    mutable std::mutex mutex_;  ///< Guards position_. Marked mutable so const getters can lock.
    int id_;                    ///< Unique robot identifier (immutable after construction).
    Vector2D position_;         ///< Current position in room coordinates (guarded by mutex_).
};

#endif // ROBOT_H
