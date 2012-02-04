#ifndef REPORTMODEL_H
#define REPORTMODEL_H

#include "idtextmodel.h"

class ReportModel : public IdTextModel
{
	Q_OBJECT

public:
    ReportModel(QObject *parent = 0);
	~ReportModel();

	QVariant data(const QModelIndex &index, int role) const;

	void load(int _regionId, const QString& content = QString()); // заполнить данными
	QString content(); 
private:
	bool loadContent(const QString& content); 
private:
	int regionId; 
};

#endif // REPORTMODEL_H
