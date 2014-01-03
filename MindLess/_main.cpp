#include <QtQml>
#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include <mindcell.h>
#include <cellmanager.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QtQuick2ApplicationViewer viewer;

    qmlRegisterType<MindCell>("org.ginosoft.qmlcomponents", 1, 0, "MindCell");

    CellManager cellManager;
    viewer.rootContext()->setContextProperty("cellManager", &cellManager);

    viewer.setMainQmlFile(QStringLiteral("qml/DontMind/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
