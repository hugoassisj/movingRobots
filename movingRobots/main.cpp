/**
 * @file main.cpp
 * @brief Application entry point for the movingRobots QML application.
 *
 * Creates the QML engine, registers the BackendController as a context
 * property, and loads the Main.qml file. All domain objects and worker
 * threads are owned by BackendController.
 *
 * Application architecture:
 *
 *   GUI thread (Qt event loop)
 *   +-- BackendController (context property "backend")
 *       +-- Robot 0, 1, 2       (Q_PROPERTY bindings read by QML)
 *       +-- Source 0, 1, 2      (used exclusively by worker threads)
 *       +-- Buffer              (shared producer-consumer queue)
 *       +-- Processor           (shared accumulator)
 *       +-- Source threads (x3) (produce readings -> Buffer)
 *       +-- Processor thread    (Buffer -> Processor -> Robot)
 *
 * Data flow:
 *   Source thread  --[produce]--> Buffer --[take]--> Processor --[setPosition]--> Robot --[signal]--> QML
 *
 * Threading:
 *   - This file runs entirely on the GUI (main) thread.
 *   - BackendController's constructor starts worker threads.
 *   - application.exec() enters the Qt event loop, which processes
 *     queued signals from worker threads and drives QML rendering.
 *   - On exit, BackendController's destructor joins all workers.
 */

#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "BackendController/backendcontroller.h"
#include "Robot/robot.h"

int main(int argc, char* argv[])
{
    QGuiApplication application(argc, argv);

    // Set the application icon (title bar + taskbar).
    application.setWindowIcon(QIcon(":/resources/app_icon.png"));

    // Use the Universal style (Windows-native look).
    QQuickStyle::setStyle("Universal");

    // Register Robot type so QML can access its properties.
    // "Uncreatable" means QML cannot instantiate Robot directly via
    // "Robot {}": instances are provided by BackendController::robots.
    // This is the correct pattern when C++ owns object lifetime.
    qmlRegisterUncreatableType<Robot>("MovingRobots", 1, 0, "Robot",
                                      "Robots are created by BackendController");

    // Create the backend controller (owns all domain objects + threads).
    // Constructed on the stack so its destructor runs automatically
    // before main() returns, ensuring all threads are joined.
    BackendController backend;

    // Set up QML engine and expose backend to QML.
    // setContextProperty makes "backend" available as a global object
    // in all QML files without requiring an import statement.
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("backend", &backend);

    // Load the main QML file from the module's resources.
    engine.loadFromModule("MovingRobots", "Main");

    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Failed to load QML module 'MovingRobots'. "
                       "Check that Main.qml exists and has no syntax errors.";
        return -1;
    }

    // Enter the Qt event loop. This drives:
    //   - QML rendering and property binding updates
    //   - Delivery of queued signals from worker threads
    //   - Timer events (buffer size polling)
    // Returns when the application window is closed.
    return application.exec();
}

