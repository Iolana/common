#include "dbase.h"
#include <QObject>
#include <QVariant>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QtDebug>
#include <QStringList>

Dbase::Dbase()
{
}

bool Dbase::setSourceName(const QString &name)
{
    if(name.isNull() || name.isEmpty())
    {
        setLastError("Empty string given");
        return false;
    }
    if(name != QString(":memory:")
            && !QFile::exists(name))
    {
        setLastError("File not found");
        return false;
    }
    mDb = QSqlDatabase::addDatabase("QSQLITE", name);
    mDb.setDatabaseName(name);
    if(openDatabase())
    {
        if(isDbApplicable())
            return true;
        else
        {
            QSqlDatabase::removeDatabase(name);
            return false;
        }
    }
    return false;
}

QString Dbase::getSourceName()
{
    return mDb.connectionName();
}

int Dbase::runQueryInsert(const QString &table, const QStringList &fields, const QList<QVariant> &values)
{
    if(table.isNull() || table.isEmpty())
    {
        setLastError("Empty string given");
        return -1;
    }
    if(fields.size() != values.size())
    {
        setLastError("Parameters count mismatch");
        return -1;
    }

    QStringList params;
    for(int i = 0; i < values.size(); ++i)
        params << "?";

    QString sql("INSERT INTO %1(%2) VALUES(%3)");
    sql = sql.arg(table)
            .arg(fields.join(", "))
            .arg(params.join(", "));
    QSqlQuery query(mDb);
    query.prepare(sql);
    for(int i = 0; i < values.size(); ++i)
        query.addBindValue(values.at(i));
    if(_execQuery(query))
    {
        QVariant lastId = query.lastInsertId();
        if(lastId.isValid())
            return lastId.toInt();
    }
    return -1;
}

bool Dbase::_execQuery(QSqlQuery &query)
{
    if(query.exec())
        return true;
    else
    {
        setLastError(QObject::tr("QUERY {%1} FAILED WITH ERROR {%2}")
                .arg(query.lastQuery())
                .arg(query.lastError().text()));
        return false;
    }
}

QString Dbase::getLastError()
{
    return mLastError;
}

bool Dbase::openDatabase()
{
    if(!mDb.open())
    {
        setLastError(mDb.lastError().text());
        return false;
    }
    QSqlQuery query(QString("PRAGMA foreign_keys = ON"), mDb);
    query.exec();
    return _execQuery(query);
}

bool Dbase::isDbApplicable()
{
    QString sql = QString("SELECT count(name) FROM sqlite_master");
    QVariant result = runQueryForValue(sql);
    if(!result.isNull()
            && result.toInt() >= 0)
        return true;
    setLastError("File is not sqlite database");
    return false;
}

void Dbase::setLastError(const QString &text)
{
    mLastError = text;
    qDebug() << mLastError;
}


QVariant Dbase::runQueryForValue(const QString &sql, const QList<QVariant> &params)
{
    QSqlQuery query(mDb);
    query.prepare(sql);
    for(int i = 0; i < params.size(); ++i)
        query.addBindValue(params.at(i));
    if(_execQuery(query)
            && query.next())
        return query.value(0);
    return QVariant();
}

QList<Dbase::Row> Dbase::runQueryForRows(const QString &sql, const QList<QVariant> &params)
{
    QList<Dbase::Row> result;
    QSqlQuery query(mDb);
    query.setForwardOnly(true);
    query.prepare(sql);
    for(int i = 0; i < params.size(); ++i)
        query.addBindValue(params.at(i));
    if(_execQuery(query))
    {
        int fieldsCount = query.record().count();
        while(query.next())
        {
            Dbase::Row row;
            for(int i = 0; i < fieldsCount; ++i)
                row.columns << query.value(i);
            result << row;
        }
    }
    return result;
}
