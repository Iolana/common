import QtQuick 2.0

Rectangle {
    width: 800
    height: 600
    ListModel {
        id: myModel
        ListElement { name: "Name 1"; value: "Value 1"; xp: 200; yp: 200 }
        ListElement { name: "Name 2"; value: "Value 2"; xp: 300; yp: 300 }
        ListElement { name: "Name 3"; value: "Value 3"; xp: 400; yp: 400 }
    }

    Repeater {
    model: myModel

        Rectangle {
            id: textFrame
            width: 200; height: 150
            border.color: "black"
            border.width: 2
            x: xp; y: yp

            Text {
                text: name + textFrame.z
                font.pointSize: 20
                anchors.centerIn: parent
            }

            MouseArea {
                anchors.fill: parent
                drag.target: textFrame
            }
        }
    }
    Rectangle {
        id: testBtn
        color: "lightsteelblue"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        height: testLabel.height + 10
        width: testLabel.width + 20
        anchors.margins: 10
        Text {
            id: testLabel
            text: qsTr("Test")
            font.pointSize: 20
            anchors.centerIn: parent
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    myModel.append({"name": "Name new", "value": "Value new"});
                }
            }
        }
    }
}
