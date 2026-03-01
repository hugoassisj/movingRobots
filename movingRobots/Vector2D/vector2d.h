#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <string>

/**
 * @file vector2d.h
 * @brief Lightweight 2D position with source and robot metadata.
 *
 * Vector2D is the primary data type exchanged between threads in this
 * application. It flows from Source threads into the Buffer, out to the
 * Processor thread, and finally into Robot objects read by the GUI thread.
 *
 * Threading: Vector2D is a value type with no internal synchronization.
 * Thread safety is the responsibility of the container or owner (Buffer,
 * Processor, Robot). It is always passed by value or by const reference
 * to avoid shared mutable state.
 */

/**
 * @brief A simple 2D position vector carrying source and robot identifiers.
 *
 * This is a plain value type (POD-like) designed to be freely copied, stored
 * in containers, and passed by value. It intentionally has NO internal
 * synchronization: thread safety must be ensured by the owner or container
 * (e.g., Buffer, Processor, Robot).
 *
 * The @c sourceId and @c robotId fields identify which source produced
 * the position and which robot it belongs to, respectively.
 */
struct Vector2D {
    int x = 0;          ///< Horizontal coordinate (pixels).
    int y = 0;          ///< Vertical coordinate (pixels).
    int sourceId = -1;  ///< ID of the source that produced this reading.
    int robotId  = -1;  ///< ID of the robot this position refers to.

    /// Sentinel value indicating an unassigned source or robot ID.
    static constexpr int INVALID_ID = -1;

    /// Default constructor: zero position, invalid IDs.
    Vector2D() = default;

    /**
     * @brief Constructs a Vector2D with explicit coordinates and optional IDs.
     * @param x        Horizontal coordinate.
     * @param y        Vertical coordinate.
     * @param sourceId Source identifier (default: INVALID_ID).
     * @param robotId  Robot identifier (default: INVALID_ID).
     */
    Vector2D(int x, int y, int sourceId = INVALID_ID, int robotId = INVALID_ID);

    /**
     * @brief Returns a human-readable string representation.
     * @return String in the format "(x=..., y=..., src=..., robot=...)".
     */
    std::string toString() const;
};

#endif // VECTOR2D_H
