import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Universal
import QtQuick.Layouts

/**
 * Main application window for the Moving Robots concurrency demo.
 *
 * Fully responsive layout — no hardcoded pixel sizes.
 * The grid maintains a 4:3 aspect ratio and scales with the window.
 */
ApplicationWindow {
    id: root

    visible: true
    title: "Moving Robots — Concurrency Demo"

    // Initial size based on screen geometry; fully resizable.
    width: Screen.width * 0.55
    height: Screen.height * 0.65
    minimumWidth: 640
    minimumHeight: 480

    Universal.theme: Universal.System

    // Keyboard navigation for the selected robot.
    Item {
        id: keyboardFocusItem
        anchors.fill: parent
        focus: true

        Keys.onUpPressed:    backend.moveSelectedRobot(0, -backend.gridStep)
        Keys.onDownPressed:  backend.moveSelectedRobot(0,  backend.gridStep)
        Keys.onLeftPressed:  backend.moveSelectedRobot(-backend.gridStep, 0)
        Keys.onRightPressed: backend.moveSelectedRobot( backend.gridStep, 0)
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: root.width * 0.015
        spacing: root.height * 0.015

        // Top section: grid + controls
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: root.width * 0.015

            // Robot grid — takes ~55% of width, maintains 4:3 aspect ratio
            RobotGrid {
                Layout.fillHeight: true
                Layout.preferredWidth: parent.width * 0.55
                focusTarget: keyboardFocusItem
            }

            // Controls — takes remaining width
            ControlPanel {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }

        // Bottom section: buffer panel
        BufferPanel {
            Layout.fillWidth: true
        }
    }
}
