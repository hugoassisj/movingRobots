import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Universal
import QtQuick.Layouts

/**
 * BufferPanel — Shows the shared buffer fill level and provides action buttons.
 *
 * Fully responsive — uses Layout.fillWidth and proportional spacing
 * with no hardcoded pixel dimensions.
 */
GroupBox {
    id: bufferPanel
    title: "Shared Buffer"

    ToolTip.visible: bufferGroupHover.hovered
    ToolTip.text: "The bounded producer-consumer buffer shared between source and processor threads"
    ToolTip.delay: 800

    HoverHandler { id: bufferGroupHover }

    ColumnLayout {
        anchors.fill: parent
        spacing: parent.height * 0.06

        // --- Buffer usage text ---
        RowLayout {
            Layout.fillWidth: true

            Label {
                text: "Items: <b>" + backend.bufferSize + "</b> / " + backend.bufferMaxSize
                textFormat: Text.RichText
            }

            Item { Layout.fillWidth: true }

            Label {
                text: {
                    var pct = backend.bufferMaxSize > 0
                        ? Math.round(100 * backend.bufferSize / backend.bufferMaxSize)
                        : 0
                    return pct + "% full"
                }
                color: {
                    var pct = backend.bufferMaxSize > 0
                        ? (100 * backend.bufferSize / backend.bufferMaxSize)
                        : 0
                    if (pct > 80) return "#F44336"
                    if (pct > 50) return "#FF9800"
                    return Universal.foreground
                }
                font.bold: true
            }
        }

        // --- Progress bar ---
        ProgressBar {
            Layout.fillWidth: true
            from: 0
            to: backend.bufferMaxSize
            value: backend.bufferSize

            ToolTip.visible: progressHover.hovered
            ToolTip.text: backend.bufferSize + " of " + backend.bufferMaxSize + " slots used"
            ToolTip.delay: 500

            HoverHandler { id: progressHover }
        }

        // --- Action buttons + selected robot ---
        RowLayout {
            Layout.fillWidth: true

            Button {
                text: "Remove Item"
                onClicked: backend.manualRemove()

                ToolTip.visible: hovered
                ToolTip.text: "Manually take one item from the buffer, process it, and update the robot position"
                ToolTip.delay: 500
            }

            Button {
                text: "Print State"
                onClicked: backend.printState()

                ToolTip.visible: hovered
                ToolTip.text: "Print the current buffer contents and processor matrix to the console (stdout)"
                ToolTip.delay: 500
            }

            Item { Layout.fillWidth: true }

            Label {
                text: "Selected: <b>Robot " + (backend.selectedRobot + 1) + "</b>"
                textFormat: Text.RichText
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                color: {
                    var colors = ["#2196F3", "#4CAF50", "#FF9800"]
                    return colors[backend.selectedRobot] || Universal.foreground
                }

                ToolTip.visible: selHover.hovered
                ToolTip.text: "Use arrow keys to move the selected robot. Click a robot on the grid to select it."
                ToolTip.delay: 500

                HoverHandler { id: selHover }
            }
        }
    }
}
