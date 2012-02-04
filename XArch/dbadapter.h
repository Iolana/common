#ifndef DBADAPTER_H
#define DBADAPTER_H

#include <QObject>
#include <QHash>
#include "structures.h"
class QSqlQuery; 

class DbAdapter : public QObject
{
	Q_OBJECT

public:
	DbAdapter(QObject *parent);
	~DbAdapter();
	static Patient getPatient(int id); 
	static Examination getExamination(int id); 
	static QStringList getDictionariesList(); 
	static QHash<int, QString> getAllModalities(); 
	static QHash<int, QString> getAllDevices(int mod); 
	static int insertDevice(const QString& device, int mod); 
	static void updateDevice(int id, const QString& device); 
	static void deleteDevice(int id); 
	static QHash<int, QString> getAllRegions(int mod); 
	static int insertRegion(const QString& region, int mod); 
	static void updateRegion(int id, const QString& region); 
	static void deleteRegion(int id); 
	static QHash<int, QString> getAllContrasts(int mod); 
	static int insertContrast(const QString& contrast, int mod); 
	static void updateContrast(int id, const QString& contrast); 
	static void deleteContrast(int id); 
	static QHash<int, QString> getAllOrders(); 
	static int insertReferral(const QString& referral); 
	static void updateReferral(int id, const QString& referral); 
	static void deleteReferral(int id); 
	static QString modalityById(int id); 
	static QString deviceById(int id); 
	static QString regionById(int id); 
	static QString contrastById(int id); 
	static QString orderedByById(int id); 
	static QList<std::pair<int, QString> > loadTemplate(int regionId); 
	static int insertPatient(const Patient& p); 
	static void updatePatient(const Patient& p); 
	static int insertExamination(const Examination& e); 
	static void updateExamination(const Examination& e); 
	static QString getReportForExam(int id); 
	static QString getConclusionForExam(int id); 
	static void updateReport(int id, const QString& text); 
	static void updateConclusion(int id, const QString& text); 
	static QString templateForModality(int id); 
	static QString getTemplateItem(int id); 
	static QString getConclusionPhrase(int id); 
	static QStringList getAltPhrases(int id); 
	static QStringList getCommonPhrases(int regionId); 
	static QStringList getConclusionPhrases(int regionId); 
	static void addCommonPhrase(int regionId, const QString& text); 
	static void addAltPhrase(int id, const QString& text); 
	static void addConclusionPhrase(int id, const QString& text); 
	static void delCommonPhrase(int regionId, const QString& text); 
	static void delAltPhrase(int id, const QString& text); 
	static void delConclusionPhrase(int id, const QString& text); 
	static bool updateTemplateItems(int regionId, const QStringList& list); 
	static int templateItemsCount(int regionId); 
	static QString templateItemText(int regionId, int index); 
	static int templateItemId(int regionId, int index); 
	static void setTemplateItemText(int regionId, int index, const QString& text); 
private:
	void createDb(); 
	void fillDictionaries(); 
	void fillTemplates(); 
	static void runQuery(QSqlQuery& query, const QString& sql); 
};

#endif // DBADAPTER_H
