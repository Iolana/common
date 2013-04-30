#include "dbase.h"
#include <QObject>
#include <QVariant>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlError>
#include <QtDebug>

Dbase::Dbase()
{
}

bool Dbase::setSource(const QString &name)
{
    mName = name;
    if(mName.isNull() || mName.isEmpty())
    {
        setLastError(QObject::tr("Source name not set"));
        return false;
    }
    if(mName == QString(":memory:")      // for test (fake) use only
            || QFile::exists(mName))    // for real database
    {
        if(!openDatabase())
            return false;
        return isDbApplicable();
    }
    else
    {
        setLastError(QObject::tr("File not found (%1)").arg(mName));
        return false;
    }
}

void Dbase::closeAndForgetSource()
{
    QSqlDatabase db = QSqlDatabase::database(mName);
    if(db.isValid() && db.isOpen())
        db.close();
    QSqlDatabase::removeDatabase(mName);
    mName = QString();
}

QString Dbase::getSourceName()
{
    return mName;
}

QSqlQuery Dbase::getQuery()
{
    QSqlQuery query(QSqlDatabase::database(mName));
    return query;
}

bool Dbase::runQuery(QSqlQuery &query, const QString &sql)
{
    if(query.exec(sql))
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
    Q_ASSERT(!mName.isEmpty());
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", mName);
    db.setDatabaseName(mName);
    if(!db.open())
    {
        setLastError(db.lastError().text());
        return false;
    }
    QSqlQuery query = getQuery();
    QString sql("PRAGMA foreign_keys = ON");
    return runQuery(query, sql);
}

bool Dbase::isDbApplicable()
{
    QSqlQuery query = getQuery();
    QString sql = QString("SELECT type FROM sqlite_master WHERE name = 'Patients'");
    if(runQuery(query, sql))
    {
        if(query.next()
                && query.value(0).toString() == QString("table"))
            return true;
    }
    return false;
}

void Dbase::setLastError(const QString &text)
{
    mLastError = text;
    qDebug() << mLastError;
}
