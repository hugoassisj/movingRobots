/**
 * @file robot.cpp
 * @brief Implementation of the Robot class.
 */

#include "robot.h"
#include "../constants.h"

#include <random>
#include <QMetaObject>
#include <QThread>

namespace {

using namespace Constants;

/**
 * @brief Generates a random grid-aligned position within the room.
 * @return A Vector2D with x in [0, ROOM_WIDTH) and y in [0, ROOM_HEIGHT),
 *         both multiples of GRID_STEP.
 */
Vector2D generateRandomPosition()
{
    static std::mt19937 rng(std::random_device{}());
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
    return id_;  // Immutable after construction — no lock needed.
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
        if (QThread::currentThread() == thread()) {
            emit positionChanged();
        } else {
            QMetaObject::invokeMethod(this, "positionChanged",
                                      Qt::QueuedConnection);
        }
    }
}
