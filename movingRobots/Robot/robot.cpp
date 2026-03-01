/**
 * @file robot.cpp
 * @brief Implementation of the Robot class.
 *
 * Threading: Robot lives on the GUI thread (main thread affinity).
 * All public methods acquire the internal mutex before touching position_.
 * Signal emission is routed through QMetaObject::invokeMethod with
 * Qt::QueuedConnection when called off-thread, so the QML engine always
 * receives property-change notifications on the event loop it expects.
 */

#include "robot.h"
#include "../constants.h"

#include <algorithm>
#include <random>
#include <QMetaObject>
#include <QThread>

namespace {

using namespace Constants;

/**
 * @brief Generates a random grid-aligned position within the room.
 *
 * @note Uses a thread_local RNG so that this function is safe to call
 *       from any thread without external synchronization.
 *
 * @return A Vector2D with x in [0, ROOM_WIDTH) and y in [0, ROOM_HEIGHT),
 *         both multiples of GRID_STEP.
 */
Vector2D generateRandomPosition()
{
    thread_local std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> xDist(0, ROOM_WIDTH  / GRID_STEP - 1);
    std::uniform_int_distribution<int> yDist(0, ROOM_HEIGHT / GRID_STEP - 1);
    return Vector2D(xDist(rng) * GRID_STEP, yDist(rng) * GRID_STEP);
}

} // anonymous namespace

Robot::Robot(int id, QObject* parent)
    : QObject(parent)
    , id_(id)
    , position_(generateRandomPosition())
{}

int Robot::robotId() const
{
    return id_;  // Immutable after construction, no lock needed.
}

int Robot::posX() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return position_.x;
}

int Robot::posY() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return position_.y;
}

Vector2D Robot::getPosition() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return position_;
}

void Robot::setPosition(const Vector2D& newPosition)
{
    bool changed = false;
    {
        // Lock scope is kept as short as possible: only the comparison
        // and assignment are serialized. The signal emission below
        // happens outside the lock to avoid holding the mutex while
        // Qt dispatches the signal (which could reenter if Direct
        // connection were used).
        std::lock_guard<std::mutex> lock(mutex_);
        if (position_.x != newPosition.x || position_.y != newPosition.y) {
            position_.x = newPosition.x;
            position_.y = newPosition.y;
            changed = true;
        }
    }

    if (changed) {
        // Ensure the signal is delivered on the GUI thread regardless of
        // which thread called setPosition().
        //
        // If we are already on the GUI thread (e.g., keyboard control via
        // moveSelectedRobot), emit directly for immediate QML update.
        // Otherwise, use QueuedConnection so the signal is delivered via
        // the GUI event loop: this is critical because QML property
        // bindings must be evaluated on the thread that owns the QObject.
        if (QThread::currentThread() == thread()) {
            emit positionChanged();
        } else {
            QMetaObject::invokeMethod(this, "positionChanged",
                                      Qt::QueuedConnection);
        }
    }
}

void Robot::clampedSetPosition(const Vector2D& target, int gridStep,
                               int roomWidth, int roomHeight)
{
    bool changed = false;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        int dx = std::clamp(target.x - position_.x, -gridStep, gridStep);
        int dy = std::clamp(target.y - position_.y, -gridStep, gridStep);
        int newX = std::clamp(position_.x + dx, 0, roomWidth  - gridStep);
        int newY = std::clamp(position_.y + dy, 0, roomHeight - gridStep);
        if (position_.x != newX || position_.y != newY) {
            position_.x = newX;
            position_.y = newY;
            changed = true;
        }
    }

    if (changed) {
        if (QThread::currentThread() == thread()) {
            emit positionChanged();
        } else {
            QMetaObject::invokeMethod(this, "positionChanged",
                                      Qt::QueuedConnection);
        }
    }
}
