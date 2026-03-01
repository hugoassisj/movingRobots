cmake_minimum_required(VERSION 3.16)
project(movingRobots VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Quick QuickControls2)

qt_standard_project_setup(REQUIRES 6.5)

qt_policy(SET QTP0004 NEW)

qt_add_executable(movingRobots
    main.cpp
    constants.h
    Robot/robot.h
    Robot/robot.cpp
    Buffer/buffer.h
    Buffer/buffer.cpp
    Source/source.h
    Source/source.cpp
    Processor/processor.h
    Processor/processor.cpp
    Vector2D/vector2d.h
    Vector2D/vector2d.cpp
    BackendController/backendcontroller.h
    BackendController/backendcontroller.cpp
)

# Embed the application icon into the Qt resource system.
qt_add_resources(movingRobots "app_resources"
    FILES resources/app_icon.png
)

set_source_files_properties(qml/Theme.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(movingRobots
    URI MovingRobots
    VERSION 1.0
    QML_FILES
        qml/Theme.qml
        qml/Main.qml
        qml/RobotGrid.qml
        qml/ControlPanel.qml
        qml/BufferPanel.qml
)

target_link_libraries(movingRobots PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::QuickControls2
)

# Link pthreads on Unix
if(UNIX)
    target_link_libraries(movingRobots PRIVATE pthread)
endif()
