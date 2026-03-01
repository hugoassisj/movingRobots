#ifndef SOURCE_H
#define SOURCE_H

#include <random>
#include "../Robot/robot.h"
#include "../Vector2D/vector2d.h"
#include "../constants.h"

/**
 * @file source.h
 * @brief Position-sensing source (e.g., IMU, GPS, Odometry).
 */

/**
 * @brief Simulates a position-sensing source for the robots.
 *
 * Each source generates a new position reading for a robot by applying
 * a fixed delta (±POSITION_DELTA) in both axes to the robot's current
 * position. The sign of the delta is chosen randomly via a per-instance
 * Mersenne Twister engine, ensuring thread safety without shared RNG state.
 *
 * After computing the raw new position, the static saturate() helper
 * clamps it to the room boundaries so robots never leave the area.
 */
class Source {
public:
    /**
     * @brief Constructs a source with the given unique identifier.
     * @param id Source index (0 = IMU, 1 = GPS, 2 = Odometry in this demo).
     */
    explicit Source(int id);

    /**
     * @brief Produces a new position reading for the given robot.
     *
     * Reads the robot's current position, applies a random +/– delta to
     * each axis, clamps to room boundaries, and tags the result with
     * this source's ID and the robot's ID.
     *
     * @param robot Reference to the target robot (read-only access).
     * @return Newly computed position with metadata.
     */
    Vector2D produce(Robot& robot);

    /**
     * @brief Clamps a position to the valid room boundaries.
     *
     * Keeps x in [0, roomWidth - GRID_STEP] and y in
     * [0, roomHeight - GRID_STEP] so that source readings near borders
     * never jump outside the arena.
     *
     * @param[in,out] position    The position to clamp in-place.
     * @param         roomWidth   Room width in pixels.
     * @param         roomHeight  Room height in pixels.
     */
    static void saturate(Vector2D& position, int roomWidth, int roomHeight);

    static constexpr int ROOM_WIDTH  = Constants::ROOM_WIDTH;   ///< Room width in pixels.
    static constexpr int ROOM_HEIGHT = Constants::ROOM_HEIGHT;  ///< Room height in pixels.

private:
    int id_;                ///< Unique source identifier.
    std::mt19937 rng_;      ///< Per-instance RNG for thread-safe randomness.

    /// Movement delta matches GRID_STEP so readings stay grid-aligned.
    static constexpr int POSITION_DELTA = Constants::GRID_STEP;
};

#endif // SOURCE_H
