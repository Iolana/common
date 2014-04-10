#include <QDebug>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "dbworker.h"

DbWorker::DbWorker(const QString& path, QObject *parent) :
    QObject(parent)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);
    db.open();
    QSqlQuery query;
    QString sql = "PRAGMA foreign_keys = ON";
    if(!query.exec(sql))
        qDebug() << query.lastError().text() << __FILE__ << ":" << __LINE__;
}

QStringList DbWorker::find(const QString &text, const QString& tag, bool notStoredOnly)
{
    QStringList list;
    if(text.isEmpty() && tag.isEmpty())
        return list;
    QSqlQuery query;
    QString sql;
    QString storedText = notStoredOnly ? QString(" and stored = ''") : QString("");
    if(!text.isEmpty()) {
        if(!tag.isEmpty())
            sql = QString("select name from Entries where name like '%%1%' and tags like '%%2%' %3 order by name")
                    .arg(text).arg(tag).arg(storedText);
        else
            sql = QString("select name from Entries where name like '%%1%' %2 order by name").arg(text).arg(storedText);
    }
    else if(!tag.isEmpty())
        sql = QString("select name from Entries where tags like '%%1%' %2 order by name").arg(tag).arg(storedText);
    if(!query.exec(sql))
        qDebug() << query.lastError().text() << __FILE__ << ":" << __LINE__;
    while(query.next())
        list << query.value(0).toString();
    return list;
}

QStringList DbWorker::findByUuid(const QString &uuid)
{
    QStringList list;
    if(uuid.isEmpty())
        return list;
    QSqlQuery query;
    QString sql = QString("select name from Entries where uuid like '%1%'").arg(uuid);
    if(!query.exec(sql))
        qDebug() << query.lastError().text() << __FILE__ << ":" << __LINE__;
    while(query.next())
        list << query.value(0).toString();
    return list;
}

void DbWorker::getData(const QString &name, QString &uuid, QString &tags, QString &stored, QString &comment)
{
    QSqlQuery query;
    QString sql = QString("select uuid, tags, stored, comment from Entries where name = '%1'")
            .arg(name);
    if(!query.exec(sql))
        qDebug() << query.lastError().text() << __FILE__ << ":" << __LINE__;
    if(query.next()) {
        uuid = query.value(0).toString();
        tags = query.value(1).toString();
        stored = query.value(2).toString();
        comment = query.value(3).toString();
    }
}

void DbWorker::getUuid(const QString &name, QString &uuid)
{
    QSqlQuery query;
    QString sql = QString("select uuid from Entries where name = '%1'")
            .arg(name);
    if(!query.exec(sql))
        qDebug() << query.lastError().text() << __FILE__ << ":" << __LINE__;
    if(query.next())
        uuid = query.value(0).toString();
}

bool DbWorker::update(const QString &oldName, const QString &newName, const QString &uuid, const QString &tags, const QString &stored, const QString &comment)
{
    QSqlQuery query;
    QString sql = QString("update Entries set\
                          name=:name, uuid=:uuid, tags=:tags, stored=:stored, comment=:comment\
            where name = :old_name");
    query.prepare(sql);
    query.bindValue(":name", newName);
    query.bindValue(":uuid", uuid);
    query.bindValue(":tags", tags);
    query.bindValue(":stored", stored);
    query.bindValue(":comment", comment);
    query.bindValue(":old_name", oldName);
    if(!query.exec()) {
        qDebug() << query.lastError().text() << __FILE__ << ":" << __LINE__;
        return false;
    }
    return true;
}

bool DbWorker::insert(const QString &name, const QString &uuid, const QString &tags, const QString &stored, const QString &comment)
{
    QSqlQuery query;
    QString sql =
    QString("insert into Entries(name, uuid, tags, stored, comment)\
            values (:name, :uuid, :tags, :stored, :comment)");
    query.prepare(sql);
    query.bindValue(":name", name);
    query.bindValue(":uuid", uuid);
    query.bindValue(":tags", tags);
    query.bindValue(":stored", stored);
    query.bindValue(":comment", comment);
    if(!query.exec()) {
        qDebug() << query.lastError().text() << __FILE__ << ":" << __LINE__;
        return false;
    }
    return true;
}

bool DbWorker::remove(const QString &name)
{
    QSqlQuery query;
    QString sql = QString("delete from Entries where name=:name");
    query.prepare(sql);
    query.bindValue(":name", name);
    if(!query.exec()) {
        qDebug() << query.lastError().text() << __FILE__ << ":" << __LINE__;
        return false;
    }
    return true;
}

