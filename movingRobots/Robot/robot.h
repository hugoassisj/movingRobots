#ifndef ROBOT_H
#define ROBOT_H

#include <mutex>
#include <QObject>
#include "../Vector2D/vector2d.h"

/**
 * @file robot.h
 * @brief Thread-safe robot exposed to QML with notifiable properties.
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
     * Safe to call from any thread — the signal is emitted via
     * QMetaObject::invokeMethod with Qt::QueuedConnection when
     * called from a non-GUI thread.
     *
     * @param newPosition New coordinates to apply.
     */
    void setPosition(const Vector2D& newPosition);

signals:
    /**
     * @brief Emitted when the robot's position changes.
     */
    void positionChanged();

private:
    mutable std::mutex mutex_;  ///< Guards all mutable state.
    int id_;                    ///< Unique robot identifier.
    Vector2D position_;         ///< Current position in room coordinates.
};

#endif // ROBOT_H
