#include <QString>
#include <QtTest>

class TestDbase : public QObject
{
    Q_OBJECT
    
public:
    TestDbase();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
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

void TestDbase::testCase1()
{
    QFETCH(QString, data);
    QVERIFY2(true, "Failure");
}

void TestDbase::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_APPLESS_MAIN(TestDbase)

#include "tst_testdbase.moc"
