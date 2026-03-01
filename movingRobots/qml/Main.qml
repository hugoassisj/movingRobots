import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

/**
 * Main application window.
 * * Now features a distinct background color with white "Cards" floating on top,
 * creating a clean, modern dashboard hierarchy.
 */
ApplicationWindow {
    id: root

    visible: true
    title: "Moving Robots"

    width: Screen.width * 0.60
    height: Screen.height * 0.70
    minimumWidth: 900
    minimumHeight: 720

    // Set the overall app background
    color: Theme.bgApp

    // Keyboard navigation
    Item {
        id: keyboardFocusItem
        focus: true
        Keys.onUpPressed:    backend.moveSelectedRobot(0, -backend.gridStep)
        Keys.onDownPressed:  backend.moveSelectedRobot(0,  backend.gridStep)
        Keys.onLeftPressed:  backend.moveSelectedRobot(-backend.gridStep, 0)
        Keys.onRightPressed: backend.moveSelectedRobot( backend.gridStep, 0)
    }

    ColumnLayout {
        id: mainColumn
        anchors.fill: parent
        anchors.margins: Theme.outerMargin
        spacing: Theme.sectionSpacing

        // -- Dashboard Header --
        RowLayout {
            Layout.fillWidth: true
            
            ColumnLayout {
                spacing: 4
                Label {
                    text: "Moving Robots"
                    font.pixelSize: Theme.fontPageTitle
                    font.weight: Font.Bold
                    color: Theme.textPrimary
                }
                Label {
                    text: "Concurrency & Thread Sync Demonstration"
                    font.pixelSize: Theme.fontBody
                    color: Theme.textSecondary
                }
            }
            Item { Layout.fillWidth: true }

            // Dark / Light theme toggle
            Rectangle {
                Layout.preferredHeight: 36
                Layout.preferredWidth: themeRow.implicitWidth + 24
                color: Theme.actionMuted
                radius: Theme.radiusPill
                border.color: Theme.border
                border.width: 1

                RowLayout {
                    id: themeRow
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 8

                    Label {
                        text: Theme.darkMode ? "\u263E" : "\u2600"
                        font.pixelSize: 16
                        color: Theme.textPrimary
                    }

                    Rectangle {
                        implicitWidth: 40; implicitHeight: 22
                        radius: height / 2
                        color: themeSwitch.checked ? Theme.switchTrackOn : Theme.switchTrackOff
                        border.color: themeSwitch.checked ? Theme.switchTrackOn : Theme.border
                        border.width: 1

                        Rectangle {
                            x: themeSwitch.checked ? parent.width - width - 3 : 3
                            anchors.verticalCenter: parent.verticalCenter
                            width: 16; height: 16; radius: 8
                            color: Theme.switchHandle
                            Behavior on x { NumberAnimation { duration: 120; easing.type: Easing.InOutQuad } }
                        }

                        MouseArea {
                            id: themeSwitch
                            anchors.fill: parent
                            property bool checked: Theme.darkMode
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                checked = !checked
                                Theme.darkMode = checked
                            }

                            ToolTip.visible: hovered
                            ToolTip.text: Theme.darkMode ? "Switch to light mode" : "Switch to dark mode"
                            ToolTip.delay: 400

                            HoverHandler { id: themeHover }
                            property bool hovered: themeHover.hovered
                        }
                    }
                }
            }
        }

        // -- Main content: map alongside controls --
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: Theme.sectionSpacing

            // Left -- Robot Map Card
            Rectangle {
                Layout.fillHeight: true
                Layout.preferredWidth: mainColumn.width * 0.55
                Layout.minimumWidth: 350
                color: Theme.bgCard
                radius: Theme.radiusCard
                border.color: Theme.border
                border.width: 1

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: Theme.contentSpacing
                    spacing: Theme.contentSpacing

                    // Header row: title + selected robot indicator
                    RowLayout {
                        Layout.fillWidth: true

                        Label {
                            text: "Live Arena"
                            font.pixelSize: Theme.fontSectionTitle
                            font.weight: Font.DemiBold
                            color: Theme.textPrimary
                        }

                        Item { Layout.fillWidth: true }

                        // Selected Robot Pill
                        Rectangle {
                            Layout.preferredHeight: 28
                            Layout.preferredWidth: selectedRow.implicitWidth + 20
                            color: Theme.robotColorsLight[backend.selectedRobot % Theme.robotColorsLight.length] || Theme.bgApp
                            radius: Theme.radiusPill
                            border.color: Theme.robotColors[backend.selectedRobot % Theme.robotColors.length] || Theme.border
                            border.width: 1

                            ToolTip.visible: selHover.hovered
                            ToolTip.text: "Use arrow keys to move. Click a robot on the grid to change selection."
                            ToolTip.delay: 500

                            HoverHandler { id: selHover }

                            RowLayout {
                                id: selectedRow
                                anchors.centerIn: parent
                                spacing: 6

                                Rectangle {
                                    width: 8; height: 8; radius: 4
                                    color: Theme.robotColors[backend.selectedRobot % Theme.robotColors.length] || Theme.textPrimary
                                }
                                Label {
                                    text: "Robot " + (backend.selectedRobot + 1)
                                    font.weight: Font.DemiBold
                                    font.pixelSize: Theme.fontMeta
                                    color: Theme.textPrimary
                                }
                            }
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: Theme.bgApp
                        radius: Theme.radiusControl
                        border.color: Theme.border
                        border.width: 1

                        RobotGrid {
                            anchors.fill: parent
                            anchors.margins: 4
                            focusTarget: keyboardFocusItem
                        }
                    }
                }
            }

            // Right -- Controls (Positions & Settings)
            ControlPanel {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumWidth: 380
            }
        }

        // -- Bottom: Shared Buffer Card --
        BufferPanel {
            Layout.fillWidth: true
        }
    }
}