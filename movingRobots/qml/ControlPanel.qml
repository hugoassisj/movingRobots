import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

/**
 * ControlPanel -- Split into clean, individual cards.
 * Data presentation is improved using "pills" for coordinates.
 */
Item {
    id: controlRoot

    ColumnLayout {
        anchors.fill: parent
        spacing: Theme.sectionSpacing

        // =================================================================
        // Card 1: Live Coordinates
        // =================================================================
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 190
            color: Theme.bgCard
            radius: Theme.radiusCard
            border.color: Theme.border
            border.width: 1

            ToolTip.visible: coordsHover.hovered
            ToolTip.text: "Live X/Y coordinates of each robot on the grid"
            ToolTip.delay: 800

            HoverHandler { id: coordsHover }

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: Theme.contentSpacing
                spacing: 4

                Label {
                    text: "Live Coordinates"
                    font.pixelSize: Theme.fontSectionTitle
                    font.weight: Font.DemiBold
                    color: Theme.textPrimary
                }

                // Header Row
                RowLayout {
                    Layout.fillWidth: true
                    Label { text: "Unit"; font.pixelSize: Theme.fontMeta; color: Theme.textSecondary; font.weight: Font.Medium; Layout.preferredWidth: 60 }
                    Label { text: "X-Axis"; font.pixelSize: Theme.fontMeta; color: Theme.textSecondary; font.weight: Font.Medium; Layout.fillWidth: true }
                    Label { text: "Y-Axis"; font.pixelSize: Theme.fontMeta; color: Theme.textSecondary; font.weight: Font.Medium; Layout.fillWidth: true }
                }

                Rectangle { Layout.fillWidth: true; height: 1; color: Theme.border }

                // Dynamic Rows
                Repeater {
                    model: backend.robots

                    RowLayout {
                        Layout.fillWidth: true
                        
                        // Robot Dot + Name
                        RowLayout {
                            Layout.preferredWidth: 60
                            spacing: Theme.unit
                            Rectangle {
                                width: 10; height: 10; radius: 5
                                color: Theme.robotColors.length > 0
                                       ? Theme.robotColors[index % Theme.robotColors.length]
                                       : "gray"
                            }
                            Label {
                                text: "R" + (index + 1)
                                font.pixelSize: Theme.fontBody
                                font.weight: Font.Medium
                                color: Theme.textPrimary
                            }
                        }

                        // X Coordinate Pill
                        Rectangle {
                            Layout.fillWidth: true; Layout.preferredHeight: 28
                            color: Theme.bgApp; radius: Theme.radiusControl
                            Label {
                                anchors.centerIn: parent
                                text: Math.round(modelData.posX)
                                font.pixelSize: Theme.fontBody
                                color: Theme.textPrimary
                            }
                        }

                        // Y Coordinate Pill
                        Rectangle {
                            Layout.fillWidth: true; Layout.preferredHeight: 28
                            color: Theme.bgApp; radius: Theme.radiusControl
                            Label {
                                anchors.centerIn: parent
                                text: Math.round(modelData.posY)
                                font.pixelSize: Theme.fontBody
                                color: Theme.textPrimary
                            }
                        }
                    }
                }
            }
        }

        // =================================================================
        // Card 2: Thread Controls
        // =================================================================
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 200
            color: Theme.bgCard
            radius: Theme.radiusCard
            border.color: Theme.border
            border.width: 1

            ToolTip.visible: threadCardHover.hovered
            ToolTip.text: "Configure source and processor threads"
            ToolTip.delay: 800

            HoverHandler { id: threadCardHover }

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: Theme.contentSpacing
                spacing: Theme.itemSpacing

                Label {
                    text: "Thread Configuration"
                    font.pixelSize: Theme.fontSectionTitle
                    font.weight: Font.DemiBold
                    color: Theme.textPrimary
                    Layout.bottomMargin: Theme.unit
                }

                // ---- Reusable themed switch ----
                Component {
                    id: themedSwitch
                    Switch {
                        id: sw
                        font.pixelSize: Theme.fontBody
                        font.weight: Font.Medium
                        Layout.preferredWidth: 130

                        indicator: Rectangle {
                            implicitWidth: 40; implicitHeight: 22
                            x: sw.leftPadding
                            y: parent.height / 2 - height / 2
                            radius: height / 2
                            color: sw.checked ? Theme.switchTrackOn : Theme.switchTrackOff
                            border.color: sw.checked ? Theme.switchTrackOn : Theme.border
                            border.width: 1

                            Rectangle {
                                x: sw.checked ? parent.width - width - 3 : 3
                                anchors.verticalCenter: parent.verticalCenter
                                width: 16; height: 16; radius: 8
                                color: Theme.switchHandle
                                Behavior on x { NumberAnimation { duration: 120; easing.type: Easing.InOutQuad } }
                            }
                        }

                        contentItem: Text {
                            text: sw.text
                            font: sw.font
                            color: Theme.textPrimary
                            leftPadding: sw.indicator.width + sw.spacing
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }

                // Helper to style modern sliders
                Component {
                    id: modernSlider
                    Slider {
                        id: control
                        background: Rectangle {
                            x: control.leftPadding; y: control.topPadding + control.availableHeight / 2 - height / 2
                            implicitWidth: 150; implicitHeight: 6
                            width: control.availableWidth; height: implicitHeight
                            radius: 3; color: Theme.border
                            Rectangle {
                                width: control.visualPosition * parent.width; height: parent.height
                                color: Theme.actionMain; radius: 3
                            }
                        }
                        handle: Rectangle {
                            x: control.leftPadding + control.visualPosition * (control.availableWidth - width)
                            y: control.topPadding + control.availableHeight / 2 - height / 2
                            implicitWidth: 16; implicitHeight: 16; radius: 8
                            color: control.pressed ? Theme.actionHover : Theme.actionMain
                        }
                    }
                }

                // Source 0 (IMU) Thread Row
                RowLayout {
                    Layout.fillWidth: true
                    spacing: Theme.itemSpacing

                    Loader {
                        sourceComponent: themedSwitch
                        Layout.preferredWidth: 130
                        onLoaded: {
                            item.text = "IMU";
                            item.checked = Qt.binding(function(){ return backend.source0Enabled; });
                            item.toggled.connect(function(){ backend.source0Enabled = item.checked; });
                        }

                        ToolTip.visible: hovered
                        ToolTip.text: "Enable or disable Source 1 (IMU) thread"
                        ToolTip.delay: 500
                    }
                    Loader {
                        sourceComponent: modernSlider
                        Layout.fillWidth: true
                        onLoaded: {
                            item.from = 10; item.to = 1000; item.stepSize = 10;
                            item.value = Qt.binding(function(){ return backend.source0Delay; });
                            item.moved.connect(function(){ backend.source0Delay = item.value; });
                        }

                        ToolTip.visible: hovered
                        ToolTip.text: "Delay between IMU capture cycles (ms)"
                        ToolTip.delay: 500
                    }
                    Label {
                        text: backend.source0Delay + " ms"
                        font.pixelSize: Theme.fontMeta; color: Theme.textSecondary
                        Layout.preferredWidth: 55; horizontalAlignment: Text.AlignRight
                    }
                }

                // Source 1 (GPS) Thread Row
                RowLayout {
                    Layout.fillWidth: true
                    spacing: Theme.itemSpacing

                    Loader {
                        sourceComponent: themedSwitch
                        Layout.preferredWidth: 130
                        onLoaded: {
                            item.text = "GPS";
                            item.checked = Qt.binding(function(){ return backend.source1Enabled; });
                            item.toggled.connect(function(){ backend.source1Enabled = item.checked; });
                        }

                        ToolTip.visible: hovered
                        ToolTip.text: "Enable or disable Source 2 (GPS) thread"
                        ToolTip.delay: 500
                    }
                    Loader {
                        sourceComponent: modernSlider
                        Layout.fillWidth: true
                        onLoaded: {
                            item.from = 10; item.to = 1000; item.stepSize = 10;
                            item.value = Qt.binding(function(){ return backend.source1Delay; });
                            item.moved.connect(function(){ backend.source1Delay = item.value; });
                        }

                        ToolTip.visible: hovered
                        ToolTip.text: "Delay between GPS capture cycles (ms)"
                        ToolTip.delay: 500
                    }
                    Label {
                        text: backend.source1Delay + " ms"
                        font.pixelSize: Theme.fontMeta; color: Theme.textSecondary
                        Layout.preferredWidth: 55; horizontalAlignment: Text.AlignRight
                    }
                }

                // Source 2 (Odometry) Thread Row
                RowLayout {
                    Layout.fillWidth: true
                    spacing: Theme.itemSpacing

                    Loader {
                        sourceComponent: themedSwitch
                        Layout.preferredWidth: 130
                        onLoaded: {
                            item.text = "Odometry";
                            item.checked = Qt.binding(function(){ return backend.source2Enabled; });
                            item.toggled.connect(function(){ backend.source2Enabled = item.checked; });
                        }

                        ToolTip.visible: hovered
                        ToolTip.text: "Enable or disable Source 3 (Odometry) thread"
                        ToolTip.delay: 500
                    }
                    Loader {
                        sourceComponent: modernSlider
                        Layout.fillWidth: true
                        onLoaded: {
                            item.from = 10; item.to = 1000; item.stepSize = 10;
                            item.value = Qt.binding(function(){ return backend.source2Delay; });
                            item.moved.connect(function(){ backend.source2Delay = item.value; });
                        }

                        ToolTip.visible: hovered
                        ToolTip.text: "Delay between Odometry capture cycles (ms)"
                        ToolTip.delay: 500
                    }
                    Label {
                        text: backend.source2Delay + " ms"
                        font.pixelSize: Theme.fontMeta; color: Theme.textSecondary
                        Layout.preferredWidth: 55; horizontalAlignment: Text.AlignRight
                    }
                }

                // Processor Thread Row
                RowLayout {
                    Layout.fillWidth: true
                    spacing: Theme.itemSpacing

                    Loader {
                        sourceComponent: themedSwitch
                        Layout.preferredWidth: 130
                        onLoaded: {
                            item.text = "Processor";
                            item.checked = Qt.binding(function(){ return backend.processorEnabled; });
                            item.toggled.connect(function(){ backend.processorEnabled = item.checked; });
                        }

                        ToolTip.visible: hovered
                        ToolTip.text: "Enable or disable the Processor thread"
                        ToolTip.delay: 500
                    }
                    Loader {
                        sourceComponent: modernSlider
                        Layout.fillWidth: true
                        onLoaded: {
                            item.from = 10; item.to = 1000; item.stepSize = 10;
                            item.value = Qt.binding(function(){ return backend.processorDelay; });
                            item.moved.connect(function(){ backend.processorDelay = item.value; });
                        }

                        ToolTip.visible: hovered
                        ToolTip.text: "Delay between processing cycles (ms)"
                        ToolTip.delay: 500
                    }
                    Label {
                        text: backend.processorDelay + " ms"
                        font.pixelSize: Theme.fontMeta; color: Theme.textSecondary
                        Layout.preferredWidth: 55; horizontalAlignment: Text.AlignRight
                    }
                }
            }
        }
    }
}