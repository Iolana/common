#ifndef DBASE_H
#define DBASE_H

#include <QString>
#include <QSqlQuery>

class Dbase
{
public:
    Dbase();
    bool setSource(const QString& name);
    void closeAndForgetSource();
    QString getSourceName();
    QSqlQuery getQuery();
    bool runQuery(QSqlQuery& query, const QString& sql = QString());
    QString getLastError();

private:
    bool openDatabase();
    bool isDbApplicable();
    void setLastError(const QString& text);

private:
    QString mName;
    QString mLastError;
};

#endif // DBASE_H
