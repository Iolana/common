#include "Speller.h"
#include "hunspell/hunspell.hxx"
#include <QSettings>
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>

Speller::Speller(QObject *parent) :
    QObject(parent)
{
    QSettings settings(QString("%1.ini").arg(QCoreApplication::applicationName()), QSettings::IniFormat);
    settings.beginGroup("Hunspell");
    QString affixes = settings.value("affixes", QString()).toString();
    QString dictionary = settings.value("dictionary", QString()).toString();
    QString mUserDictPath = settings.value("userDictionary", QString()).toString();
    settings.endGroup();

    mHunspell = new Hunspell(affixes.toLocal8Bit().constData(), dictionary.toLocal8Bit().constData());

    QFile dictFile(mUserDictPath);
    if(dictFile.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&dictFile);
        for(QString word = stream.readLine(); !word.isEmpty(); word = stream.readLine())
            mHunspell->add(word.toUtf8().constData());
        dictFile.close();
    }

}
bool Speller::isWrong(const QString& word)
{
    int result = mHunspell->spell(word.toUtf8().constData());
    if(result == 0)
        return true;
    return false;
}
void Speller::addToUserDict(const QString& word)
{
    mHunspell->add(word.toUtf8().constData());
    if(mUserDictPath.isNull() || mUserDictPath.isEmpty())
        return;
    QFile dictFile(mUserDictPath);
    if(dictFile.open(QIODevice::Append))
    {
        QTextStream stream(&dictFile);
        stream << word << "\n";
        dictFile.close();
    }
}
