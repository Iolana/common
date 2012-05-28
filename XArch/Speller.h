#ifndef SPELLER_H
#define SPELLER_H

#include <QObject>

class Hunspell;

class Speller : public QObject
{
    Q_OBJECT
public:
    explicit Speller(QObject *parent = 0);
    bool isWrong(const QString& word);
    void addToUserDict(const QString& word);
    
signals:
    
public slots:

private:
    Hunspell* mHunspell;
    QString mUserDictPath;
    
};

#endif // SPELLER_H
