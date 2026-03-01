import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

/**
 * BufferPanel -- completely restyled as a modern status card.
 * Custom progress bar and buttons replace default OS components.
 */
Rectangle {
    id: bufferPanel
    
    implicitHeight: bufferContent.implicitHeight + Theme.contentSpacing * 2
    color: Theme.bgCard
    radius: Theme.radiusCard
    border.color: Theme.border
    border.width: 1

    ToolTip.visible: bufferCardHover.hovered
    ToolTip.text: "The bounded producer-consumer buffer shared between source and processor threads"
    ToolTip.delay: 800

    HoverHandler { id: bufferCardHover }

    ColumnLayout {
        id: bufferContent
        anchors.fill: parent
        anchors.margins: Theme.contentSpacing
        spacing: Theme.contentSpacing

        // --- Header ---
        RowLayout {
            Layout.fillWidth: true
            Label {
                text: "Shared Pipeline Buffer"
                font.pixelSize: Theme.fontSectionTitle
                font.weight: Font.DemiBold
                color: Theme.textPrimary
            }
            Item { Layout.fillWidth: true }
            Label {
                text: backend.bufferSize + " / " + backend.bufferMaxSize
                font.pixelSize: Theme.fontBody
                font.weight: Font.Bold
                color: Theme.actionMain
            }
        }

        // --- Custom Progress Bar ---
        ProgressBar {
            id: bufferProgress
            value: backend.bufferSize
            to: backend.bufferMaxSize
            Layout.fillWidth: true
            Layout.preferredHeight: 12

            ToolTip.visible: progressHover.hovered
            ToolTip.text: backend.bufferSize + " of " + backend.bufferMaxSize + " slots used"
            ToolTip.delay: 500

            HoverHandler { id: progressHover }
            
            background: Rectangle {
                implicitHeight: 12
                color: Theme.bgApp
                radius: height / 2
            }
            contentItem: Item {
                implicitHeight: 12
                Rectangle {
                    width: bufferProgress.visualPosition * parent.width
                    height: parent.height
                    radius: height / 2
                    color: {
                        var ratio = bufferProgress.to > 0 ? bufferProgress.value / bufferProgress.to : 0
                        if (ratio >= 0.9) return "#EF4444" // Red if nearly full
                        if (ratio >= 0.7) return "#F59E0B" // Orange if getting full
                        return Theme.actionMain // Default
                    }
                    
                    Behavior on width { NumberAnimation { duration: 150; easing.type: Easing.OutQuad } }
                }
            }
        }

        // --- Actions ---
        RowLayout {
            Layout.fillWidth: true
            spacing: Theme.contentSpacing
            Layout.topMargin: Theme.unit

            // Custom Button Styling Component
            Component {
                id: customBtnStyle
                Rectangle {
                    implicitHeight: 36
                    implicitWidth: 120
                    radius: Theme.radiusControl
                }
            }

            Button {
                text: "Remove Item"
                onClicked: backend.manualRemove()

                ToolTip.visible: hovered
                ToolTip.text: "Manually take one item from the buffer and process it"
                ToolTip.delay: 500

                background: Rectangle {
                    implicitHeight: 36; implicitWidth: 120; radius: Theme.radiusControl
                    color: parent.down ? Theme.actionHover : Theme.actionMain
                }
                contentItem: Text {
                    text: parent.text; color: Theme.btnTextOnAction
                    font.weight: Font.Medium; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                }
            }

            Button {
                text: "Print State"
                onClicked: backend.printState()

                ToolTip.visible: hovered
                ToolTip.text: "Print buffer contents and processor matrix to console"
                ToolTip.delay: 500

                background: Rectangle {
                    implicitHeight: 36; implicitWidth: 120; radius: Theme.radiusControl
                    color: parent.down ? Theme.actionMutedH : Theme.actionMuted
                    border.color: Theme.border
                }
                contentItem: Text {
                    text: parent.text; color: Theme.textPrimary
                    font.weight: Font.Medium; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                }
            }

            Item { Layout.fillWidth: true }
        }
    }
}