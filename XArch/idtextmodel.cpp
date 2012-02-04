#include "idtextmodel.h"

IdTextModel::IdTextModel(QObject *parent)
	: QAbstractListModel(parent)
{

}

IdTextModel::~IdTextModel()
{

}
void IdTextModel::setHash(const QHash<int, QString>& hash)
{
	QList<std::pair<int, QString> > list; 
	QHash<int, QString>::const_iterator i = hash.constBegin(); 
	while(i != hash.constEnd())
	{
		list.append(std::make_pair(i.key(), i.value())); 
		++i; 
	}
	setList(list); 
}
void IdTextModel::setList(const QList<std::pair<int, QString> >& list)
{
	mList = list; 
	reset(); 
}
QList<std::pair<int, QString> > IdTextModel::toList()
{
	return mList; 
}
int IdTextModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
	return mList.count();
}
QVariant IdTextModel::data(const QModelIndex &index, int role) const
{
	if (index.row() < 0 || index.row() >= rowCount())
        return QVariant();
	switch(role)
	{
	case Qt::DisplayRole:
	case Qt::EditRole:
		return mList.at(index.row()).second;
	case Qt::UserRole:
		return mList.at(index.row()).first;
	}
    return QVariant();
}
Qt::ItemFlags IdTextModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return QAbstractItemModel::flags(index) | Qt::ItemIsDropEnabled;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}
bool IdTextModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (index.row() < 0 || index.row() >= rowCount())
        return false;
	switch(role)
	{
	case Qt::EditRole:
	case Qt::DisplayRole:
		mList[index.row()].second = value.toString(); 
		break; 
	case Qt::UserRole:
		mList[index.row()].first = value.toInt(); 
		break; 
	}
	emit dataChanged(index, index);
	return true;
}
bool IdTextModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (count < 1 || row < 0 || row > rowCount(parent))
        return false;
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i)
		mList.insert(row, std::make_pair(-1, QString()));
    endInsertRows();
    return true;
}
bool IdTextModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (count <= 0 || row < 0 || (row + count) > rowCount(parent))
        return false;
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i)
        mList.removeAt(row);
    endRemoveRows();
    return true;
}
Qt::DropActions IdTextModel::supportedDropActions() const
{
    return QAbstractItemModel::supportedDropActions() | Qt::MoveAction;
}