#ifndef CONSTANTS_H
#define CONSTANTS_H

/**
 * @file constants.h
 * @brief Project-wide constants shared across all modules.
 *
 * Centralises values that were previously duplicated in robot.cpp,
 * source.h, processor.cpp, and backendcontroller.h so they stay in sync.
 *
 * Threading: all values are compile-time constants (constexpr), so they
 * are inherently safe to read from any thread without synchronization.
 */

namespace Constants {

/// Room / arena width in logical pixels.
constexpr int ROOM_WIDTH  = 400;

/// Room / arena height in logical pixels.
constexpr int ROOM_HEIGHT = 300;

/// Grid cell size in logical pixels. All robot positions must be multiples of this.
constexpr int GRID_STEP   = 20;

/// Number of robots tracked by the system.
constexpr int NUM_ROBOTS  = 3;

/// Number of position sources (one per sensor type).
constexpr int NUM_SOURCES = 3;

/// Maximum number of items the shared buffer can hold.
constexpr int BUFFER_SIZE = 300;

} // namespace Constants

#endif // CONSTANTS_H
