/**
 * @file source.cpp
 * @brief Implementation of the Source position-sensing simulator.
 */

#include "source.h"
#include "../constants.h"

Source::Source(int id)
    : id_(id), rng_(std::random_device{}())
{}

void Source::saturate(Vector2D& position, int roomWidth, int roomHeight)
{
    // Clamp to valid grid range [0, dimension - GRID_STEP].
    // Clamping (not wrapping) prevents source readings near borders from
    // jumping to the opposite side, which would corrupt the average.
    constexpr int G = Constants::GRID_STEP;

    if (position.x < 0)              position.x = 0;
    else if (position.x > roomWidth  - G) position.x = roomWidth  - G;

    if (position.y < 0)              position.y = 0;
    else if (position.y > roomHeight - G) position.y = roomHeight - G;
}

Vector2D Source::produce(Robot& robot)
{
    // Pick one random cardinal direction (up / down / left / right)
    // so the robot moves to exactly one adjacent cell per reading.
    std::uniform_int_distribution<int> direction(0, 3);

    Vector2D currentPosition = robot.getPosition();

    Vector2D newPosition = currentPosition;
    switch (direction(rng_)) {
        case 0: newPosition.x += POSITION_DELTA; break;  // right
        case 1: newPosition.x -= POSITION_DELTA; break;  // left
        case 2: newPosition.y += POSITION_DELTA; break;  // down
        case 3: newPosition.y -= POSITION_DELTA; break;  // up
    }

    // Ensure the new position stays within room bounds.
    saturate(newPosition, ROOM_WIDTH, ROOM_HEIGHT);

    // Tag the reading with source and robot identifiers.
    newPosition.sourceId = id_;
    newPosition.robotId  = robot.robotId();

    return newPosition;
}
