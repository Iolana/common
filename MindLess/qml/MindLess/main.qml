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
    Item {
        id: surface
        anchors.fill: parent
        Repeater {
            model: myModel

            Rectangle {
                id: textFrame
                width: 200; height: 150
                border.color: "blue"
                border.width: 2
                radius: 10
                x: xp; y: yp
                z: 1

                Text {
                    text: name
                    font.pointSize: 20
                    anchors.centerIn: parent
                }

                MouseArea {
                    anchors.fill: parent
                    drag.target: textFrame
                    onEntered: {
                        var surfaceChilds = surface.children;
                        for(var i = 0; i < surfaceChilds.length; ++i) {
                            surfaceChilds[i].z = 1;
                        }
                        textFrame.z = 2;
                        statusText.text = "z: " + textFrame.z + "; childs: " + surfaceChilds.length; // DEBUG
                    }
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
    Text {
        id: statusText
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: parent.width - testBtn.width
        horizontalAlignment: Text.AlignHCenter
        text: "<b>Alles</b> ok<br/>. Nicht wahr?<ul>List:<li>1</li><li>2</li><li>3</li></ul>"
    }

    MessageBox {
        id: msgBox
        z: 100
    }
}
