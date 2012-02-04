#include "dbadapter.h"
#include <QtSql\QtSql>
#include <QDomDocument>
#include <QMessageBox>
#include <utility>

DbAdapter::DbAdapter(QObject *parent)
	: QObject(parent)
{
	bool mustCreateDb = !QFile::exists(QCoreApplication::applicationDirPath() + "\\data.db"); 
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(QCoreApplication::applicationDirPath() + "\\data.db");
	db.open();
	QSqlQuery query; 
	runQuery(query, "PRAGMA foreign_keys = ON"); 
	if(mustCreateDb)
		createDb(); 
}

DbAdapter::~DbAdapter()
{
	QSqlDatabase::database().close(); 
}
void DbAdapter::createDb()
{
	QSqlQuery query; 
	QSqlDatabase::database().transaction(); 
	QFile sqlFile(QCoreApplication::applicationDirPath() + "\\startup\\schema.sql"); 
	if(!sqlFile.open(QIODevice::Text | QIODevice::ReadOnly))
	{
		QMessageBox::critical(0, tr("Error"), tr("Unable to open schema.sql")); 
		return; 
	}
	QTextStream in(&sqlFile); 
	QString sql; 
	while(!in.atEnd())
	{
		QString line = in.readLine(); 
		if(!line.isEmpty())
			sql += line; 
		else if(!sql.isEmpty())
		{
			runQuery(query, sql); 
			sql.clear(); 
		}
	}
	if(!sql.isEmpty())
		runQuery(query, sql); 
	QSqlDatabase::database().commit(); 
	sqlFile.close(); 
	fillDictionaries(); 
	fillTemplates(); 
}
void DbAdapter::fillTemplates()
{
	QString path = QCoreApplication::applicationDirPath() + "\\startup\\templates.xml"; 
	QFile xmlFile(path);
	if (!xmlFile.open(QIODevice::ReadOnly))
	{
		QMessageBox::critical(0, tr("Error"), tr("Cant open %1").arg(path)); 
		return;
	}

	QDomDocument doc; 
	QString errorStr;
	int errorLine;
	int errorColumn;
	if (!doc.setContent(&xmlFile, true, &errorStr, &errorLine,
						&errorColumn)) 
	{
		QMessageBox::critical(0, tr("Error"),
						tr("Parse error at line %1, column %2:\n%3")
						.arg(errorLine)
						.arg(errorColumn)
						.arg(errorStr));
		xmlFile.close(); 
		return;
	}
	QSqlQuery query; 
	QSqlDatabase::database().transaction(); 
	QDomElement root = doc.documentElement(); 
	QDomElement regionItem = root.firstChildElement("region");
	while (!regionItem.isNull()) 
	{
		QString regionId = regionItem.attribute("id"); 
		QDomElement recordItem = regionItem.firstChildElement("item"); 
		while(!recordItem.isNull())
		{
			QString sql = QString("insert into TemplateItems(regionId, idx, value) values(%1, %2, \"%3\")")
				.arg(regionId)
				.arg(recordItem.attribute("index"))
				.arg(recordItem.attribute("value")); 
			runQuery(query, sql); 
			int recordItemIndex = -1; 
			QDomElement altItem = recordItem.firstChildElement("alt"); 
			while(!altItem.isNull())
			{
				if(recordItemIndex == -1)
				{
					runQuery(query, "select last_insert_rowid()"); 
					if(query.next())
						recordItemIndex = query.value(0).toInt(); 
					if(recordItemIndex == 0 || recordItemIndex == -1)
					{
						QMessageBox::critical(0, tr("Error"), tr("last_insert_rowid() failed")); 
						return; 
					}
				}
				QString innerSql = QString("insert into AltTemplateItems(parentId, value) values(%1, \"%2\")")
					.arg(recordItemIndex)
					.arg(altItem.attribute("value")); 
				runQuery(query, innerSql); 
				altItem = altItem.nextSiblingElement("alt"); 
			}
			recordItem = recordItem.nextSiblingElement("item"); 
		}
		regionItem = regionItem.nextSiblingElement("region");
	}
	QSqlDatabase::database().commit(); 
}
void DbAdapter::fillDictionaries()
{
	QString path = QCoreApplication::applicationDirPath() + "\\startup\\directories.xml"; 
	QFile xmlFile(path);
	if (!xmlFile.open(QIODevice::ReadOnly))
	{
		QMessageBox::critical(0, tr("Error"), tr("Cant open %1").arg(path)); 
		return;
	}

	QDomDocument doc; 
	QString errorStr;
	int errorLine;
	int errorColumn;
	if (!doc.setContent(&xmlFile, true, &errorStr, &errorLine,
						&errorColumn)) 
	{
		QMessageBox::critical(0, tr("Error"),
						tr("Parse error at line %1, column %2:\n%3")
						.arg(errorLine)
						.arg(errorColumn)
						.arg(errorStr));
		xmlFile.close(); 
		return;
	}
	QSqlQuery query; 
	QSqlDatabase::database().transaction(); 
	QDomElement root = doc.documentElement(); 
	QDomElement tableItem = root.firstChildElement("table");
	while (!tableItem.isNull()) 
	{
		QString tableName = tableItem.attribute("name"); 
		QDomElement recordItem = tableItem.firstChildElement("item"); 
		while(!recordItem.isNull())
		{
			QStringList attrNames; 
			QStringList attrValues; 
			QDomNamedNodeMap attrs = recordItem.attributes(); 
			for(int i = 0; i < attrs.count(); ++i)
			{
				attrNames << attrs.item(i).nodeName(); 
				attrValues << QString("\"") + attrs.item(i).nodeValue() + QString("\""); 
			}
			QString sql = QString("insert into %1(%2) values(%3)")
				.arg(tableName)
				.arg(attrNames.join(", "))
				.arg(attrValues.join(", ")); 
			runQuery(query, sql); 
			recordItem = recordItem.nextSiblingElement("item"); 
		}
		tableItem = tableItem.nextSiblingElement("table");
	}
	QSqlDatabase::database().commit(); 
}
void DbAdapter::runQuery(QSqlQuery& query, const QString& sql)
{
	if(!query.exec(sql))
		QMessageBox::critical(0, tr("Error"), query.lastQuery() + ": " + query.lastError().text()); 
}
QStringList DbAdapter::getDictionariesList()
{
	QStringList list; 
	QSqlQuery query; 
	runQuery(query, "select tableName from Dictionaries"); 
	while(query.next())
		list << query.value(0).toString(); 
	return list; 
}
QHash<int, QString> DbAdapter::getAllModalities()
{
	QHash<int, QString> list; 
	QSqlQuery query; 
	runQuery(query, "select id, name from Modalities"); 
	while(query.next())
		list.insert(query.value(0).toInt(), query.value(1).toString()); 
	return list; 
}
QHash<int, QString> DbAdapter::getAllDevices(int mod)
{
	QHash<int, QString> list; 
	QSqlQuery query; 
	runQuery(query, QString("select id, name from Devices where modality = %1").arg(mod)); 
	while(query.next())
		list.insert(query.value(0).toInt(), query.value(1).toString()); 
	return list; 
}
QHash<int, QString> DbAdapter::getAllRegions(int mod)
{
	QHash<int, QString> list; 
	QSqlQuery query; 
	runQuery(query, QString("select id, name from Regions where modality = %1").arg(mod)); 
	while(query.next())
		list.insert(query.value(0).toInt(), query.value(1).toString()); 
	return list; 
}
QHash<int, QString> DbAdapter::getAllContrasts(int mod)
{
	QHash<int, QString> list; 
	QSqlQuery query; 
	runQuery(query, QString("select id, name from Contrasts where modality = %1").arg(mod)); 
	while(query.next())
		list.insert(query.value(0).toInt(), query.value(1).toString()); 
	return list; 
}
QHash<int, QString> DbAdapter::getAllOrders()
{
	QHash<int, QString> list; 
	QSqlQuery query; 
	runQuery(query, "select id, name from OrderedBy"); 
	while(query.next())
		list.insert(query.value(0).toInt(), query.value(1).toString()); 
	return list; 
}

int DbAdapter::insertDevice(const QString& device, int mod)
{
	QSqlQuery query; 
	QString sql = QString("insert into Devices(name, modality) values('%1', %2)").arg(device).arg(mod); 
	runQuery(query, sql); 
	runQuery(query, "select last_insert_rowid()"); 
	if(query.next())
		return query.value(0).toInt(); 
	else
		return -1; 
}
void DbAdapter::updateDevice(int id, const QString& device)
{
	QSqlQuery query; 
	QString sql = QString("update Devices set name = '%1' where id = %2").arg(device).arg(id); 
	runQuery(query, sql); 
}
void DbAdapter::deleteDevice(int id)
{
	QSqlQuery query; 
	QString sql = QString("delete from Devices where id = %2").arg(id); 
	runQuery(query, sql); 
}
int DbAdapter::insertRegion(const QString& region, int mod)
{
	QSqlQuery query; 
	QString sql = QString("insert into Regions(name, modality) values('%1', %2)").arg(region).arg(mod); 
	runQuery(query, sql); 
	runQuery(query, "select last_insert_rowid()"); 
	if(query.next())
		return query.value(0).toInt(); 
	else
		return -1; 
}
void DbAdapter::updateRegion(int id, const QString& region)
{
	QSqlQuery query; 
	QString sql = QString("update Regions set name = '%1' where id = %2").arg(region).arg(id); 
	runQuery(query, sql); 
}
void DbAdapter::deleteRegion(int id)
{
	QSqlQuery query; 
	QString sql = QString("delete from Regions where id = %2").arg(id); 
	runQuery(query, sql); 
}
int DbAdapter::insertContrast(const QString& contrast, int mod)
{
	QSqlQuery query; 
	QString sql = QString("insert into Contrasts(name, modality) values('%1', %2)").arg(contrast).arg(mod); 
	runQuery(query, sql); 
	runQuery(query, "select last_insert_rowid()"); 
	if(query.next())
		return query.value(0).toInt(); 
	else
		return -1; 
}
void DbAdapter::updateContrast(int id, const QString& contrast)
{
	QSqlQuery query; 
	QString sql = QString("update Contrasts set name = '%1' where id = %2").arg(contrast).arg(id); 
	runQuery(query, sql); 
}
void DbAdapter::deleteContrast(int id)
{
	QSqlQuery query; 
	QString sql = QString("delete from Contrasts where id = %2").arg(id); 
	runQuery(query, sql); 
}
int DbAdapter::insertReferral(const QString& referral)
{
	QSqlQuery query; 
	QString sql = QString("insert into OrderedBy(name) values('%1')").arg(referral); 
	runQuery(query, sql); 
	runQuery(query, "select last_insert_rowid()"); 
	if(query.next())
		return query.value(0).toInt(); 
	else
		return -1; 
}
void DbAdapter::updateReferral(int id, const QString& referral)
{
	QSqlQuery query; 
	QString sql = QString("update OrderedBy set name = '%1' where id = %2").arg(referral).arg(id); 
	runQuery(query, sql); 
}
void DbAdapter::deleteReferral(int id)
{
	QSqlQuery query; 
	QString sql = QString("delete from OrderedBy where id = %2").arg(id); 
	runQuery(query, sql); 
}


int DbAdapter::insertPatient(const Patient& p)
{
	QSqlQuery query; 
	query.prepare("insert into Patients(name, birthday, sex) values(?, ?, ?)"); 
	query.addBindValue(p.name); 
	query.addBindValue(p.birthday); 
	query.addBindValue(p.sex); 
	if(!query.exec())
	{
		QMessageBox::critical(0, tr("Error"), query.lastQuery() + ": " + query.lastError().text()); 
		return -1; 
	}
	int id; 
	query.exec("select last_insert_rowid()"); 
	if(query.next())
		id = query.value(0).toInt(); 
	return id; 
}
void DbAdapter::updatePatient(const Patient& p)
{
	QSqlQuery query; 
	query.prepare("update Patients set name = ?, birthday = ?, sex = ? where id = ?"); 
	query.addBindValue(p.name); 
	query.addBindValue(p.birthday); 
	query.addBindValue(p.sex); 
	query.addBindValue(p.id); 
	if(!query.exec())
		QMessageBox::critical(0, tr("Error"), query.lastQuery() + ": " + query.lastError().text()); 
}
int DbAdapter::insertExamination(const Examination& e)
{
	QSqlQuery query; 
	query.prepare("insert into Examinations(patient, dt, modality, device, region, contrast, contrastAmount, orderedBy) \
						  values(?, ?, ?, ?, ?, ?, ?, ?)"); 
	query.addBindValue(e.patientId);
	query.addBindValue(e.examDate);
	query.addBindValue(e.modalityId);
	query.addBindValue(e.deviceId);
	query.addBindValue(e.regionId);
	query.addBindValue(e.contrastId);
	query.addBindValue(e.contrastAmount);
	query.addBindValue(e.orderedById); 
	if(!query.exec())
	{
		QMessageBox::critical(0, "Error", query.lastQuery() + ": " + query.lastError().text()); 
		return -1; 
	}
	int id; 
	query.exec("select last_insert_rowid()"); 
	if(query.next())
		id = query.value(0).toInt(); 
	return id; 
}
void DbAdapter::updateExamination(const Examination& e)
{
	QSqlQuery query; 
	query.prepare("update Examinations set patient = ?, dt = ?, modality = ?, device = ?, region = ?, contrast = ?, contrastAmount = ?, orderedBy = ? \
						  where id = ?"); 
	query.addBindValue(e.patientId);
	query.addBindValue(e.examDate);
	query.addBindValue(e.modalityId);
	query.addBindValue(e.deviceId);
	query.addBindValue(e.regionId);
	query.addBindValue(e.contrastId);
	query.addBindValue(e.contrastAmount);
	query.addBindValue(e.orderedById); 
	query.addBindValue(e.id); 
	if(!query.exec())
		QMessageBox::critical(0, "Error", query.lastQuery() + ": " + query.lastError().text()); 
}
QString DbAdapter::modalityById(int id)
{
	QSqlQuery query; 
	QString sql = QString("select name from Modalities where id = %1").arg(id); 
	runQuery(query, sql); 
	if(query.next())
		return query.value(0).toString(); 
	else
		return QString(); 
}
QString DbAdapter::deviceById(int id)
{
	QSqlQuery query; 
	QString sql = QString("select name from Devices where id = %1").arg(id); 
	runQuery(query, sql); 
	if(query.next())
		return query.value(0).toString(); 
	else
		return QString(); 
}
QString DbAdapter::regionById(int id)
{
	QSqlQuery query; 
	QString sql = QString("select name from Regions where id = %1").arg(id); 
	runQuery(query, sql); 
	if(query.next())
		return query.value(0).toString(); 
}
QString DbAdapter::contrastById(int id)
{
	QSqlQuery query; 
	QString sql = QString("select name from Contrasts where id = %1").arg(id); 
	runQuery(query, sql); 
	if(query.next())
		return query.value(0).toString(); 
}
QString DbAdapter::orderedByById(int id)
{
	QSqlQuery query; 
	QString sql = QString("select name from OrderedBy where id = %1").arg(id); 
	runQuery(query, sql); 
	if(query.next())
		return query.value(0).toString(); 
}
QList<std::pair<int, QString> > DbAdapter::loadTemplate(int regionId)
{
	QList<std::pair<int, QString> > list;
	QSqlQuery query; 
	runQuery(query, QString("select id, value from TemplateItems where regionId = %1 order by idx asc").arg(regionId)); 
	while(query.next())
		list.append(std::make_pair(query.value(0).toInt(), query.value(1).toString())); 
	return list; 
}
QString DbAdapter::getReportForExam(int id)
{
	QSqlQuery query; 
	QString sql = QString("select content from Examination where id = %1").arg(id); 
	runQuery(query, sql); 
	if(query.next())
		return query.value(0).toString(); 
	else
		return QString(); 
}
QString DbAdapter::getConclusionForExam(int id)
{
	QSqlQuery query; 
	QString sql = QString("select conclusion from Examination where id = %1").arg(id); 
	runQuery(query, sql); 
	if(query.next())
		return query.value(0).toString(); 
	else
		return QString(); 
}
void DbAdapter::updateReport(int id, const QString& text)
{
	QSqlQuery query; 
	query.prepare("update Examinations set content = ? \
						  where id = ?"); 
	query.addBindValue(text);
	query.addBindValue(id); 
	if(!query.exec())
		QMessageBox::critical(0, "Error", query.lastQuery() + ": " + query.lastError().text()); 
}
void DbAdapter::updateConclusion(int id, const QString& text)
{
	QSqlQuery query; 
	query.prepare("update Examinations set conclusion = ? \
						  where id = ?"); 
	query.addBindValue(text);
	query.addBindValue(id); 
	if(!query.exec())
		QMessageBox::critical(0, "Error", query.lastQuery() + ": " + query.lastError().text()); 
}
Patient DbAdapter::getPatient(int id)
{
	Patient pat; 
	pat.id = id; 
	QSqlQuery query; 
	QString sql = QString("select name, birthday, sex from Patients where id = %1").arg(id); 
	runQuery(query, sql); 
	if(query.next())
	{
		pat.name = query.value(0).toString(); 
		pat.birthday = query.value(1).toDate(); 
		pat.sex = query.value(2).toInt(); 
	}
	return pat; 
}
Examination DbAdapter::getExamination(int id)
{
	Examination exam; 
	exam.id = id; 
	QSqlQuery query; 
	QString sql = QString("select patient, dt, modality, device, region, contrast, contrastAmount, orderedBy, content, conclusion \
												from Examinations where id = %1").arg(id); 
	runQuery(query, sql); 
	if(query.next())
	{
		exam.patientId = query.value(0).toInt(); 
		exam.examDate = query.value(1).toDate(); 
		exam.modalityId = query.value(2).toInt(); 
		exam.deviceId = query.value(3).toInt(); 
		exam.regionId = query.value(4).toInt(); 
		exam.contrastId = query.value(5).toInt(); 
		exam.contrastAmount = query.value(6).toDouble(); 
		exam.orderedById = query.value(7).toInt(); 
		exam.content = query.value(8).toString(); 
		exam.conclusion = query.value(9).toString(); 
	}
	return exam; 
}
QString DbAdapter::templateForModality(int id)
{
	QSqlQuery query; 
	QString sql = QString("select template from Modalities where id = %1").arg(id); 
	runQuery(query, sql); 
	if(query.next())
		return query.value(0).toString(); 
	return tr("Value was not set"); 
}
QString DbAdapter::getTemplateItem(int id)
{
	QSqlQuery query; 
	QString sql = QString("select value from TemplateItems where id = %1").arg(id); 
	runQuery(query, sql); 
	if(query.next())
		return query.value(0).toString(); 
}
QString DbAdapter::getConclusionPhrase(int id)
{
	QSqlQuery query; 
	QString sql = QString("select value from ConclusionTemplateItems where id = %1").arg(id); 
	runQuery(query, sql); 
	if(query.next())
		return query.value(0).toString(); 
}
QStringList DbAdapter::getCommonPhrases(int regionId)
{
	QStringList list; 
	QSqlQuery query; 
	QString sql = QString("select value from CommonPhrases where regionId = %1").arg(regionId); 
	runQuery(query, sql); 
	while(query.next())
		list << query.value(0).toString(); 
	return list; 
}
QStringList DbAdapter::getAltPhrases(int id)
{
	QStringList list; 
	QSqlQuery query; 
	QString sql = QString("select value from AltTemplateItems where parentId = %1").arg(id); 
	runQuery(query, sql); 
	while(query.next())
		list << query.value(0).toString(); 
	return list; 
}
QStringList DbAdapter::getConclusionPhrases(int regionId)
{
	QStringList list; 
	QSqlQuery query; 
	QString sql = QString("select value from ConclusionTemplateItems where regionId = %1").arg(regionId); 
	runQuery(query, sql); 
	while(query.next())
		list << query.value(0).toString(); 
	return list; 
}
void DbAdapter::addCommonPhrase(int regionId, const QString& text)
{
	QSqlQuery query; 
	QString sql = QString("insert into CommonPhrases(regionId, value) values(%1, '%2')").arg(regionId).arg(text); 
	runQuery(query, sql); 
}
void DbAdapter::addAltPhrase(int id, const QString& text)
{
	QSqlQuery query; 
	QString sql = QString("insert into AltTemplateItems(parentId, value) values(%1, '%2')").arg(id).arg(text); 
	runQuery(query, sql); 
}
void DbAdapter::addConclusionPhrase(int regionId, const QString& text)
{
	QSqlQuery query; 
    QString sql = QString("insert into ConclusionTemplateItems(regionId, value) values(%1, '%2')").arg(regionId).arg(text);
	runQuery(query, sql); 
}
void DbAdapter::delCommonPhrase(int regionId, const QString& text)
{
	QSqlQuery query; 
	QString sql = QString("delete from CommonPhrases where regionId = %1 and value = '%2' ").arg(regionId).arg(text); 
	runQuery(query, sql); 
}
void DbAdapter::delAltPhrase(int id, const QString& text)
{
	QSqlQuery query; 
	QString sql = QString("delete from AltTemplateItems where parentId = %1 and value = '%2' ").arg(id).arg(text); 
	runQuery(query, sql); 
}
void DbAdapter::delConclusionPhrase(int regionId, const QString& text)
{
	QSqlQuery query; 
	QString sql = QString("delete from ConclusionTemplateItems where regionId = %1 and value = '%2' ").arg(regionId).arg(text); 
	runQuery(query, sql); 
}
bool DbAdapter::updateTemplateItems(int regionId, const QStringList& list)
{
	QSqlDatabase::database().transaction(); 
	QSqlQuery query; 
	QString sql = QString("delete from TemplateItems where regionId = %1").arg(regionId); 
	runQuery(query, sql); 

	sql = QString("insert into TemplateItems(regionId, idx, value) values(%1, %2, '%3')"); 
	for(int i = 0; i < list.size(); ++i)
		runQuery(query, sql.arg(regionId).arg(i).arg(list.at(i))); 

	QSqlDatabase::database().commit(); 
	return true; 
}
