import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Universal

/**
 * RobotGrid — responsive playing field with grid lines and robot markers.
 *
 * Maintains a 4:3 aspect ratio within whatever size the parent gives it.
 * Grid step and robot marker sizes scale proportionally.
 */
Item {
    id: gridRoot

    // Logical grid dimensions (driven by backend constants, not duplicated).
    readonly property int logicalWidth:  backend.roomWidth
    readonly property int logicalHeight: backend.roomHeight
    readonly property int logicalStep:   backend.gridStep

    // Scale factors: map logical coordinates → actual rendered pixels.
    readonly property real scaleX: arena.width  / logicalWidth
    readonly property real scaleY: arena.height / logicalHeight

    // Derived visual sizes.
    readonly property real cellSize: Math.min(arena.width / (logicalWidth / logicalStep),
                                              arena.height / (logicalHeight / logicalStep))

    // Item that should receive focus when a robot is clicked (set by parent).
    property Item focusTarget

    readonly property var robotColors: ["#2196F3", "#4CAF50", "#FF9800"]

    // Arena rectangle — centered, maintains 4:3 aspect ratio.
    Rectangle {
        id: arena

        readonly property real aspectRatio: 4.0 / 3.0

        width:  Math.min(parent.width, parent.height * aspectRatio)
        height: width / aspectRatio
        anchors.centerIn: parent

        color: "transparent"
        border.color: Universal.foreground
        border.width: Math.max(1, Math.round(arena.width * 0.005))

        // --- Horizontal grid lines ---
        Repeater {
            model: (logicalHeight / logicalStep) - 1
            Rectangle {
                x: 0
                y: (index + 1) * gridRoot.scaleY * logicalStep
                width: arena.width
                height: 1
                color: Qt.rgba(0.5, 0.5, 0.5, 0.3)
            }
        }

        // --- Vertical grid lines ---
        Repeater {
            model: (logicalWidth / logicalStep) - 1
            Rectangle {
                x: (index + 1) * gridRoot.scaleX * logicalStep
                y: 0
                width: 1
                height: arena.height
                color: Qt.rgba(0.5, 0.5, 0.5, 0.3)
            }
        }

        // --- Robot markers ---
        Repeater {
            model: backend.robots

            Rectangle {
                id: robotMarker

                required property var modelData
                required property int index

                x: modelData.posX * gridRoot.scaleX
                y: modelData.posY * gridRoot.scaleY
                width: gridRoot.cellSize
                height: gridRoot.cellSize
                radius: gridRoot.cellSize * 0.2
                color: robotColors[index]
                border.color: backend.selectedRobot === index ? "white" : "transparent"
                border.width: backend.selectedRobot === index
                              ? Math.max(2, gridRoot.cellSize * 0.15) : 0

                Behavior on x { NumberAnimation { duration: 80; easing.type: Easing.OutQuad } }
                Behavior on y { NumberAnimation { duration: 80; easing.type: Easing.OutQuad } }

                Text {
                    anchors.centerIn: parent
                    text: (index + 1).toString()
                    color: "white"
                    font.bold: true
                    font.pixelSize: Math.max(8, gridRoot.cellSize * 0.55)
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        backend.selectedRobot = index
                        if (gridRoot.focusTarget)
                            gridRoot.focusTarget.forceActiveFocus()
                    }
                }

                ToolTip.visible: hoverHandler.hovered
                ToolTip.text: "Robot " + (index + 1) + " — Click to select for keyboard control"
                ToolTip.delay: 500

                HoverHandler { id: hoverHandler }
            }
        }
    }
}
