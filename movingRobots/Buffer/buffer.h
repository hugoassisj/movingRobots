#ifndef BUFFER_H
#define BUFFER_H

#include <mutex>
#include <deque>
#include <string>
#include <QSemaphore>
#include "../Vector2D/vector2d.h"

/**
 * @file buffer.h
 * @brief Thread-safe bounded buffer for the producer-consumer pattern.
 */

/**
 * @brief A bounded, thread-safe FIFO buffer using semaphores.
 *
 * Implements the classic producer-consumer synchronization pattern:
 *   - @c freeSlots_ semaphore counts available write positions.
 *   - @c usedSlots_ semaphore counts items ready for consumption.
 *   - A @c std::mutex protects the underlying deque from concurrent access.
 *
 * Producers call put() or tryPut() to insert position readings.
 * Consumers call take() or tryTake() to retrieve them.
 *
 * The "try" variants accept a timeout, enabling threads to check for shutdown
 * signals periodically instead of blocking forever.
 */
class Buffer {
public:
    /**
     * @brief Constructs a buffer with the given maximum capacity.
     * @param maxSize Maximum number of elements the buffer can hold.
     */
    explicit Buffer(int maxSize);

    /**
     * @brief Inserts a position into the buffer (blocks if full).
     * @param position The position reading to insert.
     */
    void put(const Vector2D& position);

    /**
     * @brief Tries to insert a position, with a timeout.
     * @param position  The position reading to insert.
     * @param timeoutMs Maximum time to wait for a free slot (milliseconds).
     * @return true if the position was inserted, false on timeout.
     */
    bool tryPut(const Vector2D& position, int timeoutMs = 100);

    /**
     * @brief Removes and returns the oldest position (blocks if empty).
     * @return The oldest position reading in the buffer.
     */
    Vector2D take();

    /**
     * @brief Tries to remove the oldest position, with a timeout.
     * @param[out] result  Filled with the removed position on success.
     * @param timeoutMs    Maximum time to wait for an available item (ms).
     * @return true if an item was retrieved, false on timeout.
     */
    bool tryTake(Vector2D& result, int timeoutMs = 100);

    /**
     * @brief Returns the current number of items in the buffer.
     * @return Current item count.
     */
    int getSize() const;

    /**
     * @brief Returns the buffer's maximum capacity.
     * @return Maximum number of items the buffer can hold.
     */
    int getMaxSize() const;

    /**
     * @brief Returns a formatted string listing all buffered items.
     * @return Multi-line string for debugging / console output.
     */
    std::string toString() const;

private:
    mutable std::mutex mutex_;     ///< Protects @c data_ from concurrent access.
    QSemaphore freeSlots_;         ///< Counts empty slots (producer waits on this).
    QSemaphore usedSlots_;         ///< Counts filled slots (consumer waits on this).
    std::deque<Vector2D> data_;    ///< Underlying FIFO storage.
    int maxSize_;                  ///< Fixed maximum capacity.
};

#endif // BUFFER_H
