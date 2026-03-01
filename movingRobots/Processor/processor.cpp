/**
 * @file processor.cpp
 * @brief Implementation of the Processor accumulation and averaging logic.
 */

#include "processor.h"
#include "../constants.h"

#include <sstream>
#include <iostream>
#include <cmath>

Processor::Processor()
{
    // All Entry structs are value-initialized (filled = false).
}

bool Processor::insertAndProcess(const Vector2D& position, Vector2D& meanPositionOut)
{
    // Validate incoming IDs before indexing into the matrix.
    if (position.robotId  < 0 || position.robotId  >= NUM_ROBOTS ||
        position.sourceId < 0 || position.sourceId >= NUM_SOURCES) {
        std::cerr << "[Processor] Rejected position with invalid IDs: "
                  << position.toString() << std::endl;
        return false;
    }

    std::lock_guard<std::mutex> lock(mutex_);

    // Store the reading in the appropriate cell.
    Entry& cell = matrix_[position.robotId][position.sourceId];
    cell.position = position;
    cell.filled   = true;

    // Check if all sources have now reported for this robot.
    auto meanResult = tryComputeMean(position.robotId);
    if (meanResult.has_value()) {
        meanPositionOut = meanResult.value();
        resetRow(position.robotId);
        return true;
    }
    return false;
}

std::optional<Vector2D> Processor::tryComputeMean(int robotId)
{
    // Verify every source has reported.
    for (int sourceIndex = 0; sourceIndex < NUM_SOURCES; ++sourceIndex) {
        if (!matrix_[robotId][sourceIndex].filled) {
            return std::nullopt;
        }
    }

    // Compute the arithmetic mean of all source readings.
    int sumX = 0;
    int sumY = 0;
    for (int sourceIndex = 0; sourceIndex < NUM_SOURCES; ++sourceIndex) {
        sumX += matrix_[robotId][sourceIndex].position.x;
        sumY += matrix_[robotId][sourceIndex].position.y;
    }

    int meanX = sumX / NUM_SOURCES;
    int meanY = sumY / NUM_SOURCES;

    // Snap to the nearest grid cell so robots stay aligned.
    constexpr int G = Constants::GRID_STEP;
    int snappedX = static_cast<int>(std::round(static_cast<double>(meanX) / G)) * G;
    int snappedY = static_cast<int>(std::round(static_cast<double>(meanY) / G)) * G;

    // Clamp to valid grid range [0, dimension - GRID_STEP].
    snappedX = std::max(0, std::min(snappedX, Constants::ROOM_WIDTH  - G));
    snappedY = std::max(0, std::min(snappedY, Constants::ROOM_HEIGHT - G));

    return Vector2D(snappedX, snappedY,
                    Vector2D::INVALID_ID, robotId);
}

void Processor::resetRow(int robotId)
{
    for (int sourceIndex = 0; sourceIndex < NUM_SOURCES; ++sourceIndex) {
        matrix_[robotId][sourceIndex] = Entry{};
    }
}

std::string Processor::toString() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    std::ostringstream oss;
    oss << "Processor Matrix:\n";
    for (int robotIndex = 0; robotIndex < NUM_ROBOTS; ++robotIndex) {
        oss << "  Robot " << robotIndex << ": ";
        for (int sourceIndex = 0; sourceIndex < NUM_SOURCES; ++sourceIndex) {
            const Entry& cell = matrix_[robotIndex][sourceIndex];
            if (cell.filled) {
                oss << "[" << cell.position.toString() << "] ";
            } else {
                oss << "[empty] ";
            }
        }
        oss << "\n";
    }
    return oss.str();
}