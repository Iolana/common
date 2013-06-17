#include <QString>
#include <QtTest>
#include <QtSql>

#include "tst_testdbase.h"
#include "dbase.h"
#include "structures.h"

void TestDbase::initTestCase()
{
    QSqlDatabase::addDatabase("QSQLITE", "fake");
}

void TestDbase::init()
{
    foreach(QString name, QSqlDatabase::connectionNames())
    {
        if(name != QString("fake"))
            QSqlDatabase::removeDatabase(name);
    }
}

void TestDbase::setSource_realDb()
{
    int connectionsCount = QSqlDatabase::connectionNames().size();
    QString name("/home/visa/Downloads/distr/data.db");
    Dbase dbase;
    bool res = dbase.setSourceName(name);
    QVERIFY(res == true);
    QVERIFY(dbase.getSourceName() == name);
    QVERIFY(QSqlDatabase::connectionNames().contains(name) == true);
    QVERIFY(QSqlDatabase::connectionNames().size() == connectionsCount + 1);

    QSqlQuery query(QString("PRAGMA foreign_keys"), QSqlDatabase::database(name));
    query.exec();
    query.next();
    QVERIFY(query.value(0).toInt() == 1);

}

void TestDbase::setSource_inMemoryDb()
{
    int connectionsCount = QSqlDatabase::connectionNames().size();
    QString name(":memory:");
    Dbase dbase;
    bool res = dbase.setSourceName(name);
    QVERIFY(res == true);
    QVERIFY(dbase.getSourceName() == name);
    QVERIFY(QSqlDatabase::connectionNames().contains(name) == true);
    QVERIFY(QSqlDatabase::connectionNames().size() == connectionsCount + 1);

    QSqlQuery query(QString("PRAGMA foreign_keys"), QSqlDatabase::database(name));
    query.exec();
    query.next();
    QVERIFY(query.value(0).toInt() == 1);
}

void TestDbase::setSource_emptyName()
{
    int connectionsCount = QSqlDatabase::connectionNames().size();
    QString name("");
    Dbase dbase;
    bool res = dbase.setSourceName(name);
    QVERIFY(res == false);
    QVERIFY(dbase.getLastError() == QString("Empty string given"));
    QVERIFY(QSqlDatabase::connectionNames().size() == connectionsCount);
}

void TestDbase::setSource_noFile()
{
    int connectionsCount = QSqlDatabase::connectionNames().size();
    QString name("file not exists");
    Dbase dbase;
    bool res = dbase.setSourceName(name);
    QVERIFY(res == false);
    QVERIFY(dbase.getLastError() == QString("File not found"));
    QVERIFY(QSqlDatabase::connectionNames().size() == connectionsCount);
}

void TestDbase::setSource_wrongFile()
{
    int connectionsCount = QSqlDatabase::connectionNames().size();
    QString name("/home/visa/Downloads/distr/xarch.ini");
    Dbase dbase;
    bool res = dbase.setSourceName(name);
    QVERIFY(res == false);
    QVERIFY(dbase.getLastError() == QString("File is not sqlite database"));
    QVERIFY(QSqlDatabase::connectionNames().size() == connectionsCount);
}

void TestDbase::runQuery_forValue()
{
    QString name(":memory:");
    Dbase dbase;
    Q_ASSERT(dbase.setSourceName(name));

    QSqlQuery query("CREATE TABLE test_table(a text, b integer)", QSqlDatabase::database(name));
    query.exec();
    query.prepare("INSERT INTO test_table(a, b) VALUES(?, ?)");
    for(int i = 0; i < 5; ++i)
    {
        query.bindValue(0, QString("value %1").arg(i));
        query.bindValue(1, i);
        query.exec();
    }
    QVariant result = dbase.runQueryForValue("SELECT count(a) FROM test_table");
    QVERIFY(result.toInt() == 5);

    QList<QVariant> params;
    params << 1;
    result = dbase.runQueryForValue("SELECT a FROM test_table WHERE b = ?", params);
    QCOMPARE(result.toString(), QString("value 1"));

    params.clear();
    params << QString("value 1");
    result = dbase.runQueryForValue("SELECT b FROM test_table WHERE a = ?", params);
    QVERIFY(result.toInt() == 1);

    params.clear();
    params << QString("value 1");
    params << 1;
    result = dbase.runQueryForValue("SELECT b FROM test_table WHERE a = ? and b = ?", params);
    QVERIFY(result.toInt() == 1);

    params.clear();
    params << 1;
    result = dbase.runQueryForValue("SELECT b FROM test_table WHERE a = ? and b = ?", params);
    QVERIFY(result.isValid() == false);
    QCOMPARE(dbase.getLastError(),
             QString("QUERY {SELECT b FROM test_table WHERE a = ? and b = ?} FAILED WITH ERROR { Parameter count mismatch}"));

    result = dbase.runQueryForValue("SELECT a FROM test_table_ble");
    QVERIFY(result.isValid() == false);
    QCOMPARE(dbase.getLastError(),
             QString("QUERY {SELECT a FROM test_table_ble} FAILED WITH ERROR {No query Unable to fetch row}"));
}

void TestDbase::runQuery_forRows()
{
    QString name(":memory:");
    Dbase dbase;
    Q_ASSERT(dbase.setSourceName(name));

    QSqlQuery query("CREATE TABLE test_table(a text, b integer)", QSqlDatabase::database(name));
    query.exec();
    query.prepare("INSERT INTO test_table(a, b) VALUES(?, ?)");
    for(int i = 0; i < 5; ++i)
    {
        query.bindValue(0, QString("value %1").arg(i));
        query.bindValue(1, i);
        query.exec();
    }

    QList<Dbase::Row> rows = dbase.runQueryForRows("SELECT a, b FROM test_table");
    QVERIFY(rows.size() == 5);
    QVERIFY(rows.at(0).columns.size() == 2);
    for(int i = 0; i < 5; ++i)
    {
        QVERIFY(rows.at(i).columns.at(0).toString() == QString("value %1").arg(i));
        QVERIFY(rows.at(i).columns.at(1).toInt() == i);
    }

    rows.clear();
    rows = dbase.runQueryForRows("SELECT 'one', a, b FROM test_table");
    QVERIFY(rows.size() == 5);
    QVERIFY(rows.at(0).columns.size() == 3);
    for(int i = 0; i < 5; ++i)
    {
        QVERIFY(rows.at(i).columns.at(0).toString() == QString("one"));
        QVERIFY(rows.at(i).columns.at(1).toString() == QString("value %1").arg(i));
        QVERIFY(rows.at(i).columns.at(2).toInt() == i);
    }
}

void TestDbase::runQuery_insert()
{
    QString name(":memory:");
    Dbase dbase;
    Q_ASSERT(dbase.setSourceName(name));

    QSqlQuery query("CREATE TABLE test_table(id integer primary key autoincrement, a text, b integer)",
                    QSqlDatabase::database(name));
    query.exec();

    QStringList fields;
    fields << "id" << "a" << "b";
    QList<QVariant> values;
    values << 100;
    values << "value 1";
    values << 1;
    int id = dbase.runQueryInsert("test_table", fields, values);
    QCOMPARE(id, 100);

    values.clear();
    values << 200;
    values << "value 2";
    values << 2;
    id = dbase.runQueryInsert("test_table", fields, values);
    QCOMPARE(id, 200);

    query.exec("SELECT id, a, b FROM test_table");
    query.next();
    QCOMPARE(query.value(0).toInt(), 100);
    QCOMPARE(query.value(1).toString(), QString("value 1"));
    QCOMPARE(query.value(2).toInt(), 1);

    query.next();
    QCOMPARE(query.value(0).toInt(), 200);
    QCOMPARE(query.value(1).toString(), QString("value 2"));
    QCOMPARE(query.value(2).toInt(), 2);

    fields.clear();
    values.clear();
    id = dbase.runQueryInsert("", fields, values);
    QVERIFY(id == -1);
    QVERIFY(dbase.getLastError() == QString("Empty string given"));

    fields << "id" << "a" << "b";
    values << 100;
    values << "value 1";
    values << 1;
    id = dbase.runQueryInsert("test_blablabla", fields, values);
    QVERIFY(id == -1);
    QCOMPARE(dbase.getLastError(),
      QString("QUERY {INSERT INTO test_blablabla(id, a, b) VALUES(?, ?, ?)} FAILED WITH ERROR { Parameter count mismatch}"));

    fields.clear();
    values.clear();
    fields << "a" << "b";
    values << "one" << 2 << 3;
    id = dbase.runQueryInsert("test_table", fields, values);
    QVERIFY(id == -1);
    QVERIFY(dbase.getLastError() == QString("Parameters count mismatch"));

    fields.clear();
    values.clear();
    fields << "c" << "d";
    values << "one" << 300;
    id = dbase.runQueryInsert("test_table", fields, values);
    QVERIFY(id == -1);
    QCOMPARE(dbase.getLastError(),
      QString("QUERY {INSERT INTO test_table(c, d) VALUES(?, ?)} FAILED WITH ERROR { Parameter count mismatch}"));
}

void TestDbase::runQuery_update()
{
}

void TestDbase::runQuery_delete()
{
}
