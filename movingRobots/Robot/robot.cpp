#include <stdlib.h>
#include <time.h>

#include "robot.h"

using namespace std;

/**
 * @brief Constructor for the Robot class.
 *
 * Initializes a new instance of the Robot class with the given ID and a random position.
 *
 * @param id The ID of the robot.
 */
Robot::Robot(int _id)
{
    mutexLock = PTHREAD_MUTEX_INITIALIZER;

    // Seed random number generator if not already seeded
    static bool seeded = false;
    if (!seeded)
    {
        srand(time(NULL));
        seeded = true;
    }

    // Initialize robot's ID and position
    pthread_mutex_lock(&mutexLock);
    id = _id;
    pos.x = (rand() % 18) * 20; // Generate random x-coordinate within the range [0, 17] and scale it to the grid
    pos.y = (rand() % 14) * 20; // Generate random y-coordinate within the range [0, 13] and scale it to the grid
    pthread_mutex_unlock(&mutexLock);
}

/**
 * @brief Sets the position of the robot.
 *
 * @param newPos The new position of the robot.
 */
void Robot::setPosition(Vector2D newPos)
{
    pthread_mutex_lock(&mutexLock);
    pos.x = newPos.x;
    pos.y = newPos.y;
    pthread_mutex_unlock(&mutexLock);
}

/**
 * @brief Gets the current position of the robot.
 *
 * @return The current position of the robot.
 */
Vector2D Robot::getPosition()
{
    Vector2D currentPos;
    pthread_mutex_lock(&mutexLock);
    currentPos.x = pos.x;
    currentPos.y = pos.y;
    pthread_mutex_unlock(&mutexLock);
    return currentPos;
}

/**
 * @brief Gets the ID of the robot.
 *
 * @return The ID of the robot.
 */
int Robot::getID()
{
    int currentId;
    pthread_mutex_lock(&mutexLock);
    currentId = id;
    pthread_mutex_unlock(&mutexLock);
    return currentId;
}
