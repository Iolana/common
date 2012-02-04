#include "reportmodel.h"
#include <QMessageBox>
#include <QDomDocument>
#include <QTextStream>
#include "dbadapter.h"

ReportModel::ReportModel(QObject *parent)
	: IdTextModel(parent)
{
}

ReportModel::~ReportModel()
{

}
QVariant ReportModel::data(const QModelIndex &index, int role) const
{
	if(role == Qt::BackgroundRole)
	{
		int id = index.data(Qt::UserRole).toInt(); 
		if(id < 0)
			return QBrush(QColor(255, 220, 220)); 
		QString templ = DbAdapter::getTemplateItem(id); 
		if(templ == index.data().toString())
			return QVariant(); 
		else
			return QBrush(QColor(255, 220, 220)); 
	}
	else
		return IdTextModel::data(index, role); 
}
void ReportModel::load(int _regionId, const QString& content)
{
	regionId = _regionId; 
	mList.clear(); 
    // отчет еще не был создан ранее, новый
	if(content.isNull() || content.isEmpty())
	{
		QList<std::pair<int, QString> > list = DbAdapter::loadTemplate(regionId); 
		if(list.size() == 0)
			QMessageBox::critical(0, tr("Error"), tr("Template is empty.")); 
		
		for(int i = 0; i < list.size(); ++i)
			mList.append(std::make_pair(list.at(i).first, list.at(i).second)); 
	}
	else
		loadContent(content); 
	reset(); 
}
bool ReportModel::loadContent(const QString& content)
{
	QDomDocument doc; 
	QString errorStr;
	int errorLine;
	int errorColumn;
	if (!doc.setContent(content, true, &errorStr, &errorLine,
						&errorColumn)) 
	{
		QMessageBox::critical(0, tr("Error"),
						tr("Parse error at line %1, column %2:\n%3")
						.arg(errorLine)
						.arg(errorColumn)
						.arg(errorStr));
		return false;
	}
	QDomElement root = doc.documentElement(); 
	QDomElement item = root.firstChildElement("item");
	while (!item.isNull()) 
	{
		mList.append(std::make_pair(item.attribute("id").toInt(), 
			item.attribute("value"))); 
		item = item.nextSiblingElement("item");
	}
	return true; 
}
QString ReportModel::content()
{
	QDomDocument doc("content");
	QDomElement root = doc.createElement("content");
	doc.appendChild(root);
	for(int i = 0; i < mList.size(); ++i)
	{
		QDomElement item = doc.createElement("item");
		item.setAttribute("id", mList.at(i).first); 
		item.setAttribute("value", mList.at(i).second); 
		root.appendChild(item);
	}
	return doc.toString(); 
}
