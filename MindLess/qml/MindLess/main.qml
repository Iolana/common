import QtQuick 2.0
import QtQuick.Controls 1.1

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
            border.color: "blue"
            border.width: 2
            radius: 10
            x: xp; y: yp

            Text {
                text: name
                font.pointSize: 20
                anchors.centerIn: parent
            }

            MouseArea {
                anchors.fill: parent
                drag.target: textFrame
                onClicked: {
                    textFrame.z = 1;
                }
            }
        }
    }
    Button {
        id: testBtn
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        text: qsTr("Test")
        onClicked: myModel.append({"name": "Name new", "value": "Value new"});
    }
    MessageBox {
        id: msgBox
        z: 100
    }
}
