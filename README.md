# Moving Robots

[![Hits](https://hitcount.dev/p/hugoassisj/movingRobots.svg)](https://hitcount.dev/p/hugoassisj/movingRobots)
![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat-square&logo=c%2B%2B&logoColor=white)
![Qt 6](https://img.shields.io/badge/Qt-6-41CD52?style=flat-square&logo=qt&logoColor=white)
![QML](https://img.shields.io/badge/QML-UI-41CD52?style=flat-square&logo=qt&logoColor=white)
![Concurrency](https://img.shields.io/badge/Concurrency-Multithreaded-8A2BE2?style=flat-square)
![Synchronization](https://img.shields.io/badge/Synchronization-Threading-FF8C00?style=flat-square)

A **multithreaded concurrency and synchronization** demo written in **C++ / Qt 6 / QML**.

Three simulated position sources (IMU, GPS, Odometry) run on independent threads, each producing noisy readings for three robots moving inside a bounded grid. Readings are pushed into a **semaphore-based bounded buffer** (classic producer–consumer pattern), consumed by a **processor thread** that averages all three sources per robot, and then displayed in a fully responsive QML interface.

## Architecture

![Architecture](/movingRobots/resources/architecture.png)

| Component             | Role                                                                                                                    |
| --------------------- | ----------------------------------------------------------------------------------------------------------------------- |
| **Source**            | Generates random position deltas for each robot on a dedicated thread.                                                  |
| **Buffer**            | Thread-safe bounded FIFO using `QSemaphore` + `std::mutex`.                                                             |
| **Processor**         | Accumulates readings in a `[robots × sources]` matrix; computes the mean and snaps to the grid once all sources report. |
| **Robot**             | `QObject` exposing position properties to QML with cross-thread signal safety.                                          |
| **BackendController** | Owns all domain objects and threads; exposes properties and invokable actions to the QML UI.                            |

## Features

![Application screenshot](/movingRobots/resources/ui.png)

- **Card-based dashboard** with dark / light theme toggle.
- **Live arena**: 4 : 3 grid with smooth animations and click-to-select robots.
- **Live coordinates**: real-time X / Y readouts for every robot.
- **Thread controls**: enable / disable each source or the processor and adjust delays.
- **Keyboard control**: move the selected robot with arrow keys.
- **Buffer monitor**: color-coded progress bar with manual remove and print-state actions.

## Prerequisites

| Tool         | Version                                       |
| ------------ | --------------------------------------------- |
| C++ compiler | C++17 support (GCC 9+, MSVC 2019+, Clang 10+) |
| CMake        | 3.16+                                         |
| Qt           | 6.5+ (Core, Quick, QuickControls2)            |

## Building

```bash
# Configure
cmake -S movingRobots -B build -DCMAKE_PREFIX_PATH=<path-to-qt6>

# Build
cmake --build build

# Run
./build/movingRobots        # Linux / macOS
build\movingRobots.exe      # Windows
```

> **Tip:** If you use Qt Creator, open `movingRobots/CMakeLists.txt` as the project file.

## License

This project is provided for educational purposes.
