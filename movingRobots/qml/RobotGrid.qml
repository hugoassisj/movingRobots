import QtQuick
import QtQuick.Controls

/**
 * RobotGrid -- Modern card-style grid with square robots (rounded corners)
 * and hover tooltips. Maintains 4:3 aspect ratio.
 */
Item {
    id: gridRoot

    readonly property int logicalWidth:  backend.roomWidth
    readonly property int logicalHeight: backend.roomHeight
    readonly property int logicalStep:   backend.gridStep

    // Scale factors: map logical coords to rendered pixels
    readonly property real scaleX: arena.width  / logicalWidth
    readonly property real scaleY: arena.height / logicalHeight

    // Cell size derived from the arena, not the parent
    readonly property real cellSize: Math.min(arena.width / (logicalWidth / logicalStep),
                                              arena.height / (logicalHeight / logicalStep))

    property Item focusTarget: null

    // Arena -- centered, maintains 4:3 aspect ratio, with visible border
    Rectangle {
        id: arena

        readonly property real aspectRatio: logicalWidth / logicalHeight

        width:  Math.min(parent.width, parent.height * aspectRatio)
        height: width / aspectRatio
        anchors.centerIn: parent

        color: "transparent"
        border.color: Theme.border
        border.width: 1
        radius: 4

        // --- Soft Grid Lines ---
        Repeater {
            model: (logicalWidth / logicalStep) - 1
            Rectangle {
                x: (index + 1) * gridRoot.scaleX * logicalStep
                y: 0
                width: 1; height: arena.height
                color: Theme.border
            }
        }
        Repeater {
            model: (logicalHeight / logicalStep) - 1
            Rectangle {
                x: 0
                y: (index + 1) * gridRoot.scaleY * logicalStep
                width: arena.width; height: 1
                color: Theme.border
            }
        }

        // --- Square Robots (rounded corners) ---
        Repeater {
            model: backend.robots

            Rectangle {
                id: botRect

                // Top-left positioning (grid-cell aligned)
                x: modelData.posX * gridRoot.scaleX
                y: modelData.posY * gridRoot.scaleY

                width:  gridRoot.cellSize
                height: gridRoot.cellSize
                radius: gridRoot.cellSize * 0.2   // rounded corners, not circle

                color: Theme.robotColors.length > 0
                       ? Theme.robotColors[index % Theme.robotColors.length]
                       : "gray"

                // Selection ring
                border.color: backend.selectedRobot === index ? "white" : "transparent"
                border.width: backend.selectedRobot === index
                              ? Math.max(2, gridRoot.cellSize * 0.12) : 0

                // Smooth position animation
                Behavior on x { NumberAnimation { duration: 80; easing.type: Easing.OutQuad } }
                Behavior on y { NumberAnimation { duration: 80; easing.type: Easing.OutQuad } }

                // Outer glow ring when selected
                Rectangle {
                    anchors.centerIn: parent
                    width: parent.width + 8
                    height: parent.height + 8
                    radius: parent.radius + 4
                    color: "transparent"
                    border.color: Theme.robotColors.length > 0
                                   ? Theme.robotColors[index % Theme.robotColors.length]
                                   : "gray"
                    border.width: 2
                    visible: backend.selectedRobot === index
                    opacity: 0.5
                }

                Text {
                    anchors.centerIn: parent
                    text: (index + 1).toString()
                    color: "#FFFFFF"
                    font.weight: Font.Bold
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
                ToolTip.text: "Robot " + (index + 1) + " — Click to select, then use arrow keys"
                ToolTip.delay: 500

                HoverHandler { id: hoverHandler }
            }
        }
    }
}