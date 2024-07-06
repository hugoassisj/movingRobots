#include <stdlib.h>

#include "source.h"

#define NEW_POS_RANGE 20

using namespace std;

/**
 * @brief Constructs a Source object with the given ID.
 *
 * @param _id The ID of the source.
 */
Source::Source(int _id)
{
    mutexLock = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutexLock);
    id = _id;
    time = 0;
    pthread_mutex_unlock(&mutexLock);
}

/**
 * @brief Gets the ID of the source.
 *
 * @return int The ID of the source.
 */
int Source::getId()
{
    int currentId;
    pthread_mutex_lock(&mutexLock);
    currentId = id;
    pthread_mutex_unlock(&mutexLock);
    return currentId;
}

/**
 * @brief Sets the time associated with the source.
 *
 * @param newTime The time value to set.
 */
void Source::SetTime(int newTime)
{
    time = newTime;
}

/**
 * @brief Gets the time associated with the source.
 *
 * @return int The time associated with the source.
 */
int Source::GetTime()
{
    return time;
}

/**
 * @brief Adjusts the position if it goes beyond the screen boundaries.
 *
 * If the position goes beyond the screen boundaries, it adjusts it to the opposite side.
 *
 * @param pos The position vector to adjust.
 */
void Source::Saturate(Vector2D &pos)
{

    if (pos.x > 390)
    {
        pthread_mutex_lock(&mutexLock);
        pos.x = pos.x - 400;
        pthread_mutex_unlock(&mutexLock);
    }
    else if (pos.x < -10)
    {
        pthread_mutex_lock(&mutexLock);
        pos.x = 400 + pos.x;
        pthread_mutex_unlock(&mutexLock);
    }

    if (pos.y > 290)
    {
        pthread_mutex_lock(&mutexLock);
        pos.y = pos.y - 300;
        pthread_mutex_unlock(&mutexLock);
    }
    else if (pos.y < -10)
    {
        pthread_mutex_lock(&mutexLock);
        pos.y = 300 + pos.y;
        pthread_mutex_unlock(&mutexLock);
    }
}

/**
 * @brief Produces a new position for a given robot.
 *
 * @param r The robot for which to produce the position.
 * @return Vector2D The newly produced position.
 */
Vector2D Source::produce(Robot robot)
{
    Vector2D newPos;

    // Generate a random number between 0 and 1 using rand()
    bool usePositiveX = rand() % 2 == 0;
    bool usePositiveY = rand() % 2 == 0;

    pthread_mutex_lock(&mutexLock);

    newPos.x = robot.getPosition().x + (usePositiveX ? NEW_POS_RANGE : -NEW_POS_RANGE);
    newPos.y = robot.getPosition().y + (usePositiveY ? NEW_POS_RANGE : -NEW_POS_RANGE);

    pthread_mutex_unlock(&mutexLock);

    Saturate(newPos);

    // Set IDs
    newPos.sourceID = getId();
    newPos.robotID = robot.getID();

    return newPos;
}
