#ifndef DBWORKER_H
#define DBWORKER_H

#include <QObject>
#include <QStringList>

class DbWorker : public QObject
{
    Q_OBJECT
public:
    explicit DbWorker(const QString& path, QObject *parent = 0);
    QStringList find(const QString& text, const QString& tag);
    void getData(const QString& name, QString& uuid, QString& tags, QString& stored, QString& comment);
    bool update(const QString& oldName, const QString& newName, const QString& uuid,
                const QString& tags, const QString& stored, const QString& comment);

signals:

public slots:

};

#endif // DBWORKER_H
