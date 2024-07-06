#include "vector2d.h"

#define DEFAULT_VALUE 999

/**
 * @brief Constructs a Vector2D object with the specified coordinates, source ID, and robot ID.
 *
 * @param x The x-coordinate of the vector.
 * @param y The y-coordinate of the vector.
 * @param sourceID The ID of the source associated with the vector.
 * @param robotID The ID of the robot associated with the vector.
 */
Vector2D::Vector2D(int x, int y, int sourceID, int robotID)
{
    mutexLock = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutexLock);
    this->x = x;
    this->y = y;
    this->sourceID = sourceID;
    this->robotID = robotID;
    pthread_mutex_unlock(&mutexLock);
}

/**
 * @brief Constructs a Vector2D object with default values.
 *
 * Default values are set for coordinates, source ID, and robot ID.
 */
Vector2D::Vector2D()
{
    mutexLock = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutexLock);
    this->x = DEFAULT_VALUE;
    this->y = DEFAULT_VALUE;
    this->sourceID = DEFAULT_VALUE;
    this->robotID = DEFAULT_VALUE;
    pthread_mutex_unlock(&mutexLock);
}

// Set method to update the values of the vector
/**
 * @brief Sets the coordinates, source ID, and robot ID of the vector.
 *
 * @param v The Vector2D object containing the new values to set.
 */
void Vector2D::set(Vector2D vector)
{
    pthread_mutex_lock(&mutexLock);
    this->x = vector.x;
    this->y = vector.y;
    this->sourceID = vector.sourceID;
    this->robotID = vector.robotID;
    pthread_mutex_unlock(&mutexLock);
}

// Get method to retrieve the current values of the vector
/**
 * @brief Retrieves the current values of the vector.
 *
 * @return A Vector2D object containing the current coordinates, source ID, and robot ID.
 */
Vector2D Vector2D::get()
{
    Vector2D pos;
    pthread_mutex_lock(&mutexLock);
    pos.x = this->x;
    pos.y = this->y;
    pos.sourceID = this->sourceID;
    pos.robotID = this->robotID;
    pthread_mutex_unlock(&mutexLock);
    return pos;
}

/**
 * @brief Overloads the post-increment operator to increment the coordinates of the vector.
 *
 * @param int Dummy parameter to indicate post-increment operation.
 * @return `Vector2D` A copy of the vector after incrementing its coordinates.
 */
Vector2D Vector2D::operator++(int)
{
    Vector2D result(*this);
    pthread_mutex_lock(&mutexLock);
    result.x++;
    result.y++;
    pthread_mutex_unlock(&mutexLock);
    return result;
}
