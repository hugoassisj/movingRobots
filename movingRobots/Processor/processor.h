#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <mutex>
#include <optional>
#include <array>
#include <string>
#include "../Vector2D/vector2d.h"
#include "../constants.h"

/**
 * @file processor.h
 * @brief Accumulates position readings and computes per-robot averages.
 *
 * The Processor acts as the consumer side of the producer-consumer
 * pipeline. It collects one reading per source for each robot, then
 * averages them to produce a fused position estimate.
 *
 * Threading: the Processor is primarily accessed from the processor
 * worker thread (via insertAndProcess), but may also be called from
 * the GUI thread during manual buffer removal (BackendController::
 * manualRemove). A std::mutex serializes all access. The GUI thread
 * also calls toString() for debug output.
 */

/**
 * @brief Accumulates position readings from multiple sources and computes averages.
 *
 * Internally maintains a matrix of size [NUM_ROBOTS x NUM_SOURCES]. Each cell
 * holds a position reading from one source for one robot. Once all sources have
 * reported for a given robot (i.e., the row is fully populated), the processor
 * computes the arithmetic mean of the recorded positions, returns it, and
 * resets the row for the next cycle.
 *
 * This design implements a simple sensor-fusion strategy: each source provides
 * a noisy estimate, and averaging reduces the variance by a factor of
 * 1/NUM_SOURCES (assuming independent noise).
 *
 * All public operations are protected by a single mutex so that the processor
 * thread and the GUI thread can both call into this class safely.
 *
 * Ownership: Processor is a value member of BackendController. It is not
 * a QObject and has no thread affinity: thread safety comes entirely
 * from the internal mutex.
 */
class Processor {
public:
    static constexpr int NUM_ROBOTS  = Constants::NUM_ROBOTS;   ///< Number of tracked robots.
    static constexpr int NUM_SOURCES = Constants::NUM_SOURCES;  ///< Number of position sources.

    /// Default-constructs the processor with an empty matrix.
    Processor();

    /**
     * @brief Inserts a position reading and checks if all sources have reported.
     *
     * Steps:
     *   1. Validates the robot/source IDs in @p position.
     *   2. Stores the reading in the corresponding matrix cell.
     *   3. If the robot's entire row is now filled, computes the mean,
     *      writes it to @p meanPositionOut, resets the row, and returns true.
     *
     * @param position          The incoming position reading (must have valid IDs).
     * @param[out] meanPositionOut  Filled with the averaged position when the row
     *                              is complete. Only valid when the function returns true.
     * @return true  if the mean was computed (all 3 sources reported).
     * @return false if more readings are still needed, or if the IDs were invalid.
     */
    bool insertAndProcess(const Vector2D& position, Vector2D& meanPositionOut);

    /**
     * @brief Returns a multi-line string showing the current matrix state.
     * @return Formatted string for debugging / console output.
     */
    std::string toString() const;

private:
    mutable std::mutex mutex_;  ///< Guards all matrix operations.

    /// A single cell in the accumulation matrix.
    struct Entry {
        Vector2D position;  ///< Stored position reading.
        bool filled = false; ///< Whether this cell has been written in the current cycle.
    };

    /// Accumulation matrix: matrix_[robotId][sourceId].
    std::array<std::array<Entry, NUM_SOURCES>, NUM_ROBOTS> matrix_;

    /**
     * @brief Resets all cells in a robot's row to empty.
     * @param robotId Index of the row to reset.
     * @note Must be called under lock.
     */
    void resetRow(int robotId);

    /**
     * @brief Attempts to compute the mean for the given robot.
     * @param robotId Index of the row to check.
     * @return The averaged position if the row is fully populated, or std::nullopt.
     * @note Must be called under lock.
     */
    std::optional<Vector2D> tryComputeMean(int robotId);
};

#endif // PROCESSOR_H
