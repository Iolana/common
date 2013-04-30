#include <QString>
#include <QtTest>

#include "dbase.h"
#include "structures.h"

class TestDbase : public QObject
{
    Q_OBJECT
    
public:
    TestDbase();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    // database
    void dbaseSetSource();
    void dbaseQuery();


    void emptyPatient();
    //void setNewPatient();

    /*void testCase1();
    void testCase1_data();*/
};

TestDbase::TestDbase()
{
}

void TestDbase::initTestCase()
{
}

void TestDbase::cleanupTestCase()
{
}

void TestDbase::dbaseSetSource()
{
    bool res = false;
    QString name = QString();
    Dbase dbase;

    name = QString("/home/visa/Downloads/distr/data.db");   // real db source
    dbase = Dbase();
    res = dbase.setSource(name);
    QVERIFY(res == true);
    QVERIFY(dbase.getSourceName() == name);
    dbase.closeAndForgetSource();


    name = QString(":memory:");                             // in-memory source
    dbase = Dbase();
    dbase.setSource(name);
    QVERIFY(dbase.getSourceName() == name);
    dbase.closeAndForgetSource();

    name = QString("/home/visa/Downloads/distr/data.none");   // file not exists
    dbase = Dbase();
    res = dbase.setSource(name);
    QVERIFY(res == false);
    QVERIFY(dbase.getSourceName() == name);
    dbase.closeAndForgetSource();

    name = QString("/home/visa/Downloads/distr/xarch.ini");   // wrong file
    dbase = Dbase();
    res = dbase.setSource(name);
    QVERIFY(res == false);
    QVERIFY(dbase.getSourceName() == name);
    dbase.closeAndForgetSource();
}

void TestDbase::dbaseQuery()
{
    Dbase dbase;
    dbase.setSource(":memory:");
    QSqlQuery query = dbase.getQuery();
    QString sql = QString("CREATE TABLE tbl(a int)");
    QVERIFY(dbase.runQuery(query, sql) == true);

    sql = QString("ololo");
    QVERIFY(dbase.runQuery(query, sql) == false);
    QVERIFY(dbase.getLastError() == QString("near \"ololo\": syntax error Unable to execute statement"));
}

// data classes
void TestDbase::emptyPatient()
{
    Patient patient;
    QVERIFY(patient.getId() == -1);
    QVERIFY(patient.getName() == QString());
    QVERIFY(patient.getBirthday() == QDate());
    QVERIFY(patient.getSex() == Patient::SEX_UNDEFINED);
    QVERIFY(patient.getExaminationsCount() == 0);
    QVERIFY(patient.getExaminationsIds().size() == 0);
}
/*
void TestDbase::setNewPatient()
{
    int id = 3;
    QString name = QString("Иванов И.И.");
    QDate birthday = QDate(1933, 3, 3);
    Patient::Sex sex = Patient::SEX_MALE;

    Patient patient;

    patient.setId(id);
    patient.setName(name);
    patient.setBirthday(birthday);

    QVERIFY(patient.getId() == 3);
    QVERIFY(patient.getName() == QString("Иванов"));
    QVERIFY(patient.getBirthday() == QDate(1933, 3, 3));
    QVERIFY(patient.getSex() == Patient::SEX_MALE);
    QVERIFY(patient.getExaminationsCount() == 3);
    QVERIFY(patient.getExaminationsIds().size() == 3);
}
*/

/*
void TestDbase::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void TestDbase::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}*/

QTEST_APPLESS_MAIN(TestDbase)

#include "tst_testdbase.moc"
