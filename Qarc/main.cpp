#include "mdiwindow.h"
#include <QApplication>
#include <QTest>
#include "tst_testdbase.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QStringList args = a.arguments();
    if(args.contains("--test"))
    {
        args.removeAll("--test");
        TestDbase testDbase;
        return QTest::qExec(&testDbase, args);
    }
    MdiWindow w;
    w.show();
    
    return a.exec();
}
