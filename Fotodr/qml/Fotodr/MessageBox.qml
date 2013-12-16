import QtQuick 2.2

Rectangle {
    id: container
    anchors.fill: parent
    color: "lightgrey"
    visible: false

    function show(text) {
        message.text = text;
        visible = true;
    }

    Rectangle {
        id: innerContainer
        width: message.width + 60
        height: message.height + 40
        anchors.centerIn: container
        radius: 8.0
        border.width: 2


        Text {
            id: message
            font.pointSize: 20
            anchors.centerIn: innerContainer
            wrapMode: Text.WordWrap
        }


        states: [
            State {
                name: "wide_text"
                when: message.text.length > 30
                PropertyChanges {
                    target: message
                    width: 350
                }
            },
            State {
                name: "not_wide_text"
                when: message.text.length <= 30
                PropertyChanges {
                    target: message
                    width: message.paintedWidth
                }
            }

        ]

    }
    MouseArea {
        anchors.fill: container
        onClicked: container.visible = false
    }
}
