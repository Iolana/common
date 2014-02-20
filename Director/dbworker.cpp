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

QStringList DbWorker::find(const QString &text, const QString& tag)
{
    QStringList list;
    if(text.isEmpty() && tag.isEmpty())
        return list;
    QSqlQuery query;
    QString sql;
    if(!text.isEmpty()) {
        if(!tag.isEmpty())
            sql = QString("select name from Entries where name like '%%1%' and tags like '%%2%'").arg(text).arg(tag);
        else
            sql = QString("select name from Entries where name like '%%1%'").arg(text);
    }
    else if(!tag.isEmpty())
        sql = QString("select name from Entries where tags like '%%1%'").arg(tag);
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

bool DbWorker::update(const QString &oldName, const QString &newName, const QString &uuid, const QString &tags, const QString &stored, const QString &comment)
{
    QSqlQuery query;
    QString sql = QString("update Entries set\
                          name = '%1', uuid='%2', tags='%3', stored='%4', comment='%5'\
            where name = '%6'")
            .arg(newName).arg(uuid).arg(tags).arg(stored).arg(comment).arg(oldName);
    if(!query.exec(sql)) {
        qDebug() << query.lastError().text() << __FILE__ << ":" << __LINE__;
        return false;
    }
    return true;
}

bool DbWorker::insert(const QString &name, const QString &uuid, const QString &tags, const QString &stored, const QString &comment)
{
    QSqlQuery query;
    QString sql = QString("insert into Entries(name, uuid, tags, stored, comment) values\
                          ('%1', '%2', '%3', '%4', '%5')")
            .arg(name).arg(uuid).arg(tags).arg(stored).arg(comment);
    if(!query.exec(sql)) {
        qDebug() << query.lastError().text() << __FILE__ << ":" << __LINE__;
        return false;
    }
    return true;
}

bool DbWorker::remove(const QString &name)
{
    QSqlQuery query;
    QString sql = QString("delete from Entries where name='%1'").arg(name);
    if(!query.exec(sql)) {
        qDebug() << query.lastError().text() << __FILE__ << ":" << __LINE__;
        return false;
    }
    return true;
}

