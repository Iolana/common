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

}

