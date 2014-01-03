#include "cellmanager.h"
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QDebug>

CellManager::CellManager(QObject *parent) :
    QObject(parent)
{
    setup();
}

QQmlListProperty<MindCell> CellManager::cells()
{
    return QQmlListProperty<MindCell>(this, 0, &CellManager::count_cells, &CellManager::at_cells);
}

int CellManager::diagonal()
{
    return mDiagonal;
}

void CellManager::setDiagonal(int value)
{
    mDiagonal = value;
    emit diagonalChanged();
}

void CellManager::open(const QString &path)
{

}

void CellManager::save(const QString &path)
{

}

void CellManager::edit(int index)
{
    QFile tempFile(QCoreApplication::applicationDirPath() + QString("/temp_file.txt"));
    if(tempFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&tempFile);
        out << mCells.at(index)->title();
        tempFile.close();
        QProcess process;
        int result = process.execute("gvim", QStringList() << tempFile.fileName());
        qDebug() << "GVIM: " << result;
        if(tempFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in (&tempFile);
            QString newText = in.readAll();
            qDebug() << newText;
            mCells.at(index)->setTitle(newText);
        }
        else
            qDebug() << "Unable to open file.";
    }
}

MindCell *CellManager::at_cells(QQmlListProperty<MindCell> *list, int index)
{
    CellManager* manager = qobject_cast<CellManager*>(list->object);
    if(index >= 0 && index < manager->mCells.count())
        return manager->mCells.at(index);
    return 0;
}

int CellManager::count_cells(QQmlListProperty<MindCell> *list)
{
    CellManager* manager = qobject_cast<CellManager*>(list->object);
    return manager->mCells.count();
}

void CellManager::setup()
{
    setDiagonal(5);
    for(int i = 0; i < diagonal()*diagonal(); ++i) {
        MindCell* cell = new MindCell(this);
        cell->setTitle(QString("Title %1").arg(i));
        mCells.append(cell);
    }
    emit cellsChanged();
}

void CellManager::editFinisched()
{

}
