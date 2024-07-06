#include "buffer.h"
#include <iostream>

using namespace std;

/**
 * @brief Constructs a Buffer object with the given maximum size.
 *
 * @param max The maximum size of the buffer.
 */
Buffer::Buffer(int _maxSize)
{
    maxSize = _maxSize;
    mutexLock = PTHREAD_MUTEX_INITIALIZER;
}

/**
 * @brief Displays the contents of the buffer.
 */
void Buffer::display()
{
    int size;
    pthread_mutex_lock(&mutexLock);
    size = buffer.size();
    pthread_mutex_unlock(&mutexLock);

    if (size > 0)
    {
        cout << endl;
        cout << "Size: " << size << endl;
        cout << "--------------------------------------------------" << endl;

        for (int i = 0; i < size; ++i)
        {
            pthread_mutex_lock(&mutexLock);
            Vector2D &pos = buffer[i];
            cout << i + 1 << ".\tX: " << pos.x << "\tY: " << pos.y
                 << "\t|SourceID: " << pos.sourceID << "\t|RobotID: " << pos.robotID << endl;
            pthread_mutex_unlock(&mutexLock);
        }
    }
}

/**
 * @brief Initializes the buffer with default values.
 */
void Buffer::init()
{
    pthread_mutex_lock(&mutexLock);
    buffer.resize(maxSize, {-2, -2, -2, -2});
    pthread_mutex_unlock(&mutexLock);
}

/**
 * @brief Puts a position vector into the buffer.
 *
 * @param pos The position vector to be put into the buffer.
 */
void Buffer::putPositions(Vector2D pos)
{
    pthread_mutex_lock(&mutexLock);
    if (buffer.size() < maxSize)
        buffer.push_back(pos);
    pthread_mutex_unlock(&mutexLock);
}

/**
 * @brief Retrieves a position vector from the buffer.
 *
 * @return Vector2D The retrieved position vector.
 */
Vector2D Buffer::getPositions()
{
    Vector2D element;
    pthread_mutex_lock(&mutexLock);
    if (!buffer.empty())
    {
        element = buffer.front();
        buffer.pop_front();
    }
    pthread_mutex_unlock(&mutexLock);
    return element;
}

/**
 * @brief Retrieves the first position vector from the buffer without removing it.
 *
 * @return Vector2D The first position vector in the buffer.
 */
Vector2D Buffer::consult()
{
    Vector2D element;
    pthread_mutex_lock(&mutexLock);
    if (!buffer.empty())
        element = buffer.front();
    pthread_mutex_unlock(&mutexLock);
    return element;
}

/**
 * @brief Gets the current size of the buffer.
 *
 * @return int The current size of the buffer.
 */
int Buffer::getSize()
{
    int size;
    pthread_mutex_lock(&mutexLock);
    size = buffer.size();
    pthread_mutex_unlock(&mutexLock);
    return size;
}

/**
 * @brief Gets the maximum size of the buffer.
 *
 * @return int The maximum size of the buffer.
 */
int Buffer::getMaxSize()
{
    return maxSize;
}