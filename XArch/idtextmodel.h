#ifndef IDTEXTMODEL_H
#define IDTEXTMODEL_H

#include <QAbstractListModel>

class IdTextModel : public QAbstractListModel
{
	Q_OBJECT

public:
    IdTextModel(QObject *parent = 0);
	~IdTextModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    Qt::ItemFlags flags(const QModelIndex &index) const;
	Qt::DropActions supportedDropActions() const;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

	void setList(const QList<std::pair<int, QString> >& list); 
	void setHash(const QHash<int, QString>& hash); 
	QList<std::pair<int, QString> > toList(); 

protected:
	QList<std::pair<int, QString> > mList; 	
};

#endif // IDTEXTMODEL_H
