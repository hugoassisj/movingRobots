/**
 * @file buffer.cpp
 * @brief Implementation of the semaphore-based bounded Buffer.
 */

#include "buffer.h"

#include <sstream>

Buffer::Buffer(int maxSize)
    : freeSlots_(maxSize), usedSlots_(0), maxSize_(maxSize)
{}

// -- Blocking insert (waits indefinitely for a free slot) --

void Buffer::put(const Vector2D& position)
{
    freeSlots_.acquire();                           // Wait for a free slot.
    {
        std::lock_guard<std::mutex> lock(mutex_);
        data_.push_back(position);
    }
    usedSlots_.release();                           // Signal a new item.
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

Vector2D Buffer::take()
{
    usedSlots_.acquire();                           // Wait for an item.
    Vector2D element;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        element = data_.front();
        data_.pop_front();
    }
    freeSlots_.release();                           // Signal a free slot.
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