import QtQuick 2.2
import QtQuick.Dialogs 1.1
import QtQuick.Controls 1.1

Rectangle {
    width: 800
    height: 600

    Text {
        text: qsTr("Hello World")
        anchors.centerIn: parent
    }
    Button {
        id: testBtn
        text: "Test"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        onClicked: {
            showMessage("Und das ist ein Text.");
        }
    }
    Button {
        id: exitBtn
        text: "Exit"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked: Qt.quit();
    }
    MessageBox {
        id: messageBox
        anchors.centerIn: parent
    }
    function showMessage(text) {
        messageBox.show(text);
    }
}
