#ifndef REPORTCHILD_H
#define REPORTCHILD_H

#include <QWidget>
#include "ui_reportchild.h"
#include "structures.h"

class ReportModel; 

class ReportChild : public QWidget, public Ui::ReportChild
{
	Q_OBJECT

public:
	ReportChild(QWidget *parent = 0);
	~ReportChild();
	bool load(const Patient& p, const Examination& e); 
	Patient getPatient(); 
	Examination getExamination(); 
	QString userFriendlyTitle(); 
protected:
	void closeEvent(QCloseEvent* e);
private slots:
	void on_patientHeader_linkActivated(const QString& link); 
	void on_examHeader_linkActivated(const QString& link); 
	void on_conclusionText_textChanged(); 
	void on_reportList_doubleClicked(const QModelIndex& index); 
	void on_btnAdd_clicked(); 
	void on_btnDelete_clicked(); 
	void on_btnPhrases_clicked(); 
private:
	void setHeader(); 
	void save(); 
	void createContent(); 
	void editReportItem(const QModelIndex& index); 
private:
	bool saved; 
	Patient patient; 
	Examination exam; 
	ReportModel* reportModel; 
};

#endif // REPORTCHILD_H
