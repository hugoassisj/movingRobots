import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Universal
import QtQuick.Layouts

/**
 * ControlPanel — Displays robot positions and thread timing controls.
 *
 * Fully responsive — all columns use Layout.fillWidth with proportional
 * ratios instead of hardcoded pixel widths.
 */
Item {
    id: controlRoot

    ColumnLayout {
        anchors.fill: parent
        spacing: parent.height * 0.015

        // =================================================================
        // Robot Positions Group
        // =================================================================
        GroupBox {
            title: "Robot Positions"
            Layout.fillWidth: true

            ToolTip.visible: posGroupHover.hovered
            ToolTip.text: "Live X/Y coordinates of each robot on the grid"
            ToolTip.delay: 800

            HoverHandler { id: posGroupHover }

            GridLayout {
                columns: 3
                columnSpacing: parent.width * 0.04
                rowSpacing: parent.height * 0.02
                anchors.left: parent.left
                anchors.right: parent.right

                // Header row
                Label { text: "Robot"; font.bold: true; Layout.fillWidth: true; Layout.preferredWidth: 2 }
                Label { text: "Col"; font.bold: true; horizontalAlignment: Text.AlignHCenter; Layout.fillWidth: true; Layout.preferredWidth: 1 }
                Label { text: "Row"; font.bold: true; horizontalAlignment: Text.AlignHCenter; Layout.fillWidth: true; Layout.preferredWidth: 1 }

                // Robot 1
                Label { text: "Robot 1"; color: "#2196F3"; font.bold: true; Layout.fillWidth: true; Layout.preferredWidth: 2 }
                Label { text: Math.floor(backend.robots[0].posX / backend.gridStep); horizontalAlignment: Text.AlignHCenter; Layout.fillWidth: true; Layout.preferredWidth: 1 }
                Label { text: Math.floor(backend.robots[0].posY / backend.gridStep); horizontalAlignment: Text.AlignHCenter; Layout.fillWidth: true; Layout.preferredWidth: 1 }

                // Robot 2
                Label { text: "Robot 2"; color: "#4CAF50"; font.bold: true; Layout.fillWidth: true; Layout.preferredWidth: 2 }
                Label { text: Math.floor(backend.robots[1].posX / backend.gridStep); horizontalAlignment: Text.AlignHCenter; Layout.fillWidth: true; Layout.preferredWidth: 1 }
                Label { text: Math.floor(backend.robots[1].posY / backend.gridStep); horizontalAlignment: Text.AlignHCenter; Layout.fillWidth: true; Layout.preferredWidth: 1 }

                // Robot 3
                Label { text: "Robot 3"; color: "#FF9800"; font.bold: true; Layout.fillWidth: true; Layout.preferredWidth: 2 }
                Label { text: Math.floor(backend.robots[2].posX / backend.gridStep); horizontalAlignment: Text.AlignHCenter; Layout.fillWidth: true; Layout.preferredWidth: 1 }
                Label { text: Math.floor(backend.robots[2].posY / backend.gridStep); horizontalAlignment: Text.AlignHCenter; Layout.fillWidth: true; Layout.preferredWidth: 1 }
            }
        }

        // =================================================================
        // Thread Timing Group
        // =================================================================
        GroupBox {
            title: "Thread Timing"
            Layout.fillWidth: true
            Layout.fillHeight: true

            GridLayout {
                columns: 4
                columnSpacing: parent.width * 0.02
                rowSpacing: parent.height * 0.01
                anchors.left: parent.left
                anchors.right: parent.right

                // Header row — proportional column widths via preferredWidth ratios
                Label { text: "Thread";    font.bold: true; Layout.fillWidth: true; Layout.preferredWidth: 2 }
                Label { text: "Enabled";   font.bold: true; Layout.fillWidth: true; Layout.preferredWidth: 1; horizontalAlignment: Text.AlignHCenter }
                Label { text: "Delay (ms)"; font.bold: true; Layout.fillWidth: true; Layout.preferredWidth: 4; horizontalAlignment: Text.AlignHCenter }
                Label { text: "Value";     font.bold: true; Layout.fillWidth: true; Layout.preferredWidth: 2; horizontalAlignment: Text.AlignRight }

                // --- Source 1 ---
                Label {
                    text: "Source 1"; color: "#2196F3"; font.bold: true
                    Layout.fillWidth: true; Layout.preferredWidth: 2

                    ToolTip.visible: s1LabelHover.hovered
                    ToolTip.text: "IMU sensor — produces noisy position readings for all robots"
                    ToolTip.delay: 500
                    HoverHandler { id: s1LabelHover }
                }
                CheckBox {
                    checked: backend.source0Enabled
                    onToggled: backend.source0Enabled = checked
                    Layout.fillWidth: true; Layout.preferredWidth: 1
                    Layout.alignment: Qt.AlignHCenter

                    ToolTip.visible: hovered
                    ToolTip.text: "Enable or disable Source 1 (IMU) thread"
                    ToolTip.delay: 500
                }
                Slider {
                    from: 100; to: 4000; stepSize: 100
                    value: backend.source0Delay
                    onMoved: backend.source0Delay = value
                    Layout.fillWidth: true; Layout.preferredWidth: 4

                    ToolTip.visible: hovered
                    ToolTip.text: "Delay between capture cycles for Source 1 (ms)"
                    ToolTip.delay: 500
                }
                Label {
                    text: backend.source0Delay + " ms"
                    horizontalAlignment: Text.AlignRight
                    Layout.fillWidth: true; Layout.preferredWidth: 2
                }

                // --- Source 2 ---
                Label {
                    text: "Source 2"; color: "#4CAF50"; font.bold: true
                    Layout.fillWidth: true; Layout.preferredWidth: 2

                    ToolTip.visible: s2LabelHover.hovered
                    ToolTip.text: "GPS sensor — produces noisy position readings for all robots"
                    ToolTip.delay: 500
                    HoverHandler { id: s2LabelHover }
                }
                CheckBox {
                    checked: backend.source1Enabled
                    onToggled: backend.source1Enabled = checked
                    Layout.fillWidth: true; Layout.preferredWidth: 1
                    Layout.alignment: Qt.AlignHCenter

                    ToolTip.visible: hovered
                    ToolTip.text: "Enable or disable Source 2 (GPS) thread"
                    ToolTip.delay: 500
                }
                Slider {
                    from: 100; to: 4000; stepSize: 100
                    value: backend.source1Delay
                    onMoved: backend.source1Delay = value
                    Layout.fillWidth: true; Layout.preferredWidth: 4

                    ToolTip.visible: hovered
                    ToolTip.text: "Delay between capture cycles for Source 2 (ms)"
                    ToolTip.delay: 500
                }
                Label {
                    text: backend.source1Delay + " ms"
                    horizontalAlignment: Text.AlignRight
                    Layout.fillWidth: true; Layout.preferredWidth: 2
                }

                // --- Source 3 ---
                Label {
                    text: "Source 3"; color: "#FF9800"; font.bold: true
                    Layout.fillWidth: true; Layout.preferredWidth: 2

                    ToolTip.visible: s3LabelHover.hovered
                    ToolTip.text: "Odometry sensor — produces noisy position readings for all robots"
                    ToolTip.delay: 500
                    HoverHandler { id: s3LabelHover }
                }
                CheckBox {
                    checked: backend.source2Enabled
                    onToggled: backend.source2Enabled = checked
                    Layout.fillWidth: true; Layout.preferredWidth: 1
                    Layout.alignment: Qt.AlignHCenter

                    ToolTip.visible: hovered
                    ToolTip.text: "Enable or disable Source 3 (Odometry) thread"
                    ToolTip.delay: 500
                }
                Slider {
                    from: 100; to: 4000; stepSize: 100
                    value: backend.source2Delay
                    onMoved: backend.source2Delay = value
                    Layout.fillWidth: true; Layout.preferredWidth: 4

                    ToolTip.visible: hovered
                    ToolTip.text: "Delay between capture cycles for Source 3 (ms)"
                    ToolTip.delay: 500
                }
                Label {
                    text: backend.source2Delay + " ms"
                    horizontalAlignment: Text.AlignRight
                    Layout.fillWidth: true; Layout.preferredWidth: 2
                }

                // --- Processor ---
                Label {
                    text: "Processor"; font.bold: true
                    Layout.fillWidth: true; Layout.preferredWidth: 2

                    ToolTip.visible: procLabelHover.hovered
                    ToolTip.text: "Consumes buffer entries and computes averaged positions"
                    ToolTip.delay: 500
                    HoverHandler { id: procLabelHover }
                }
                CheckBox {
                    checked: backend.processorEnabled
                    onToggled: backend.processorEnabled = checked
                    Layout.fillWidth: true; Layout.preferredWidth: 1
                    Layout.alignment: Qt.AlignHCenter

                    ToolTip.visible: hovered
                    ToolTip.text: "Enable or disable the Processor thread"
                    ToolTip.delay: 500
                }
                Slider {
                    from: 10; to: 1000; stepSize: 10
                    value: backend.processorDelay
                    onMoved: backend.processorDelay = value
                    Layout.fillWidth: true; Layout.preferredWidth: 4

                    ToolTip.visible: hovered
                    ToolTip.text: "Delay between processing cycles (ms)"
                    ToolTip.delay: 500
                }
                Label {
                    text: backend.processorDelay + " ms"
                    horizontalAlignment: Text.AlignRight
                    Layout.fillWidth: true; Layout.preferredWidth: 2
                }
            }
        }
    }
}
