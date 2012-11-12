#ifndef DBTABLEMODEL_H
#define DBTABLEMODEL_H

#include <QAbstractTableModel>
#include <QStringList>

class DbTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit DbTableModel(QObject *parent = 0);
    
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool load(const QStringList& columns, const QList<QStringList>& data); 
    
private:
    QStringList mColumns; 
    QList<QStringList> mData;
    
};

#endif // DBTABLEMODEL_H
