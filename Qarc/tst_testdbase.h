#ifndef TST_TESTDBASE_H
#define TST_TESTDBASE_H

#include <QObject>

class TestDbase : public QObject
{
    Q_OBJECT

public:
    TestDbase(){}

private Q_SLOTS:
    void initTestCase();
    void init();
    // database
    void setSource_realDb();
    void setSource_inMemoryDb();
    void setSource_emptyName();
    void setSource_noFile();
    void setSource_wrongFile();
    // queries
    void runQuery_forValue();
    void runQuery_forRows();
    void runQuery_insert();
    void runQuery_update();
    void runQuery_delete();
};

#endif // TST_TESTDBASE_H
