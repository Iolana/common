#ifndef DBASE_H
#define DBASE_H

#include <QString>
#include <QVariant>
#include <QSqlDatabase>

class QSqlQuery;

class Dbase
{
public:
    struct Row { QList<QVariant> columns; };
public:
    Dbase();
    bool setSourceName(const QString& name);
    QString getSourceName();
    int runQueryInsert(const QString& table, const QStringList& fields, const QList<QVariant>& values);
    bool runQueryUpdate(const QString& sql, const QList<QVariant>& values, const QList<QVariant>& params);
    bool runQueryDelete(const QString& sql, const QList<QVariant>& params);
    QVariant runQueryForValue(const QString& sql, const QList<QVariant>& params = QList<QVariant>());
    QList<Row> runQueryForRows(const QString& sql, const QList<QVariant>& params = QList<QVariant>());
    QString getLastError();

private:
    bool _execQuery(QSqlQuery& query);
    bool openDatabase();
    bool isDbApplicable();
    void setLastError(const QString& text);

private:
    QSqlDatabase mDb;
    QString mLastError;
};

#endif // DBASE_H
