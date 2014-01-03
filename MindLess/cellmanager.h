#ifndef CELLMANAGER_H
#define CELLMANAGER_H

#include <QObject>
#include <QQmlListProperty>
#include <mindcell.h>

class CellManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int diagonal READ diagonal WRITE setDiagonal NOTIFY diagonalChanged)
    Q_PROPERTY(QQmlListProperty<MindCell> cells READ cells  NOTIFY cellsChanged)
public:
    explicit CellManager(QObject *parent = 0);

    // props
    QQmlListProperty<MindCell> cells();
    int diagonal();
    void setDiagonal(int value);

    // open/save
public slots:
    void open(const QString& path);
    void save(const QString& path);
    void edit(int index);

signals:
    void cellsChanged();
    void diagonalChanged();

private:
    static MindCell* at_cells(QQmlListProperty<MindCell>* list, int index);
    static int count_cells(QQmlListProperty<MindCell>* list);

    //test-only stuff
    void setup();

private slots:
    void editFinisched();
private:
    int mDiagonal;
    QList<MindCell*> mCells;

};

#endif // CELLMANAGER_H
