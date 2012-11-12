/*
    QList<QVariant> columnsViewWidths; 
    columnsViewWidths = settings.value("columnsView/widths", columnsViewWidths).toList(); 
	if(!columnsViewWidths.isEmpty())
	for(int i = 0; i < columnsViewWidths.size(); ++i)
		ui.columnsView->setColumnWidth(i, columnsViewWidths.at(i).toInt()); 

 **/

#include "DbTableModel.h"
#include "QSize"

DbTableModel::DbTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int DbTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mData.count(); 
}

int DbTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mColumns.count(); 
}

QVariant DbTableModel::data(const QModelIndex &index, int role) const
{
    int row = index.row(); 
    int col = index.column(); 
    if(row >= rowCount() ||
            col >= columnCount() ||
            row < 0 ||
            col < 0)
        return QVariant(); 
    
    switch (role)
    {
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignLeft | Qt::AlignVCenter); 
    case Qt::ToolTipRole:
    case Qt::DisplayRole:
        return mData.at(row).at(col); 
    default:
        return QVariant(); 
    }
}

QVariant DbTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            if(section >= 0 && section < columnCount())
                return mColumns.at(section); 
        }
    }
    return QVariant(); 
}

bool DbTableModel::load(const QStringList& columns, const QList<QStringList> &data)
{
    beginResetModel(); 
    mColumns.clear();
    mData.clear();
    
    mColumns = columns; 
    foreach(QStringList l, data)
        mData << l; 
    endResetModel(); 
    return true;
}
