/**
 * @file main.cpp
 * @brief Application entry point for the movingRobots QML application.
 *
 * Creates the QML engine, registers the BackendController as a context
 * property, and loads the Main.qml file. All domain objects and worker
 * threads are owned by BackendController.
 */

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "BackendController/backendcontroller.h"
#include "Robot/robot.h"

int main(int argc, char* argv[])
{
    QGuiApplication application(argc, argv);

    // Use the Universal style (Windows-native look).
    QQuickStyle::setStyle("Universal");

    // Register Robot type so QML can access its properties.
    qmlRegisterUncreatableType<Robot>("MovingRobots", 1, 0, "Robot",
                                      "Robots are created by BackendController");

    // Create the backend controller (owns all domain objects + threads).
    BackendController backend;

    // Set up QML engine and expose backend to QML.
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("backend", &backend);

    // Load the main QML file from the module's resources.
    engine.loadFromModule("MovingRobots", "Main");

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return application.exec();
}

