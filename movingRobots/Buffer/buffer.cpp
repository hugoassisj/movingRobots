/**
 * @file buffer.cpp
 * @brief Implementation of the semaphore-based bounded Buffer.
 *
 * Threading: this file implements the core producer-consumer
 * synchronization. The operation ordering is critical:
 *
 *   put():  acquire(freeSlots) -> lock(mutex) -> push -> unlock -> release(usedSlots)
 *   take(): acquire(usedSlots) -> lock(mutex) -> pop  -> unlock -> release(freeSlots)
 *
 * The semaphore acquire happens BEFORE the mutex lock. This means threads
 * block on the semaphore (cheap, OS-level wait) rather than spinning on
 * the mutex. The mutex is only held for the brief push/pop operation,
 * minimizing contention.
 *
 * The semaphore release happens AFTER the mutex unlock. This ensures the
 * new item is fully visible before a waiting thread is unblocked.
 */

#include "buffer.h"

#include <sstream>
#include <stdexcept>

Buffer::Buffer(int maxSize)
    : freeSlots_(maxSize), usedSlots_(0), maxSize_(maxSize)
{
    if (maxSize <= 0) {
        throw std::invalid_argument("Buffer maxSize must be positive");
    }
}

// -- Blocking insert (waits indefinitely for a free slot) --
// Ordering: semaphore acquire -> mutex lock -> push -> mutex unlock -> semaphore release.
// This two-phase protocol ensures that at most maxSize_ items exist in the
// deque at any time, and that producers block when the buffer is full.

void Buffer::put(const Vector2D& position)
{
    freeSlots_.acquire();                           // Block until a slot is available.
    {
        std::lock_guard<std::mutex> lock(mutex_);   // Brief critical section.
        data_.push_back(position);
    }
    usedSlots_.release();                           // Wake a blocked consumer, if any.
}

// -- Timed insert (returns false if no slot within timeout) --

bool Buffer::tryPut(const Vector2D& position, int timeoutMs)
{
    if (!freeSlots_.tryAcquire(1, timeoutMs)) {
        return false;
    }
    {
        std::lock_guard<std::mutex> lock(mutex_);
        data_.push_back(position);
    }
    usedSlots_.release();
    return true;
}

// -- Blocking remove (waits indefinitely for an item) --
// Mirror of put(): semaphore acquire -> mutex lock -> pop -> mutex unlock -> semaphore release.

Vector2D Buffer::take()
{
    usedSlots_.acquire();                           // Block until an item is available.
    Vector2D element;
    {
        std::lock_guard<std::mutex> lock(mutex_);   // Brief critical section.
        element = data_.front();
        data_.pop_front();
    }
    freeSlots_.release();                           // Wake a blocked producer, if any.
    return element;
}

// -- Timed remove (returns false if no item within timeout) --

bool Buffer::tryTake(Vector2D& result, int timeoutMs)
{
    if (!usedSlots_.tryAcquire(1, timeoutMs)) {
        return false;
    }
    {
        std::lock_guard<std::mutex> lock(mutex_);
        result = data_.front();
        data_.pop_front();
    }
    freeSlots_.release();
    return true;
}

int Buffer::getSize() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return static_cast<int>(data_.size());
}

int Buffer::getMaxSize() const
{
    return maxSize_;
}

std::string Buffer::toString() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    std::ostringstream oss;
    oss << "Buffer [" << data_.size() << "/" << maxSize_ << "]:\n";
    int index = 1;
    for (const auto& position : data_) {
        oss << "  " << index++ << ". " << position.toString() << "\n";
    }
    return oss.str();
}