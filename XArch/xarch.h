#ifndef XARCH_H
#define XARCH_H

#include <QtGui/QMainWindow>
#include "structures.h"

class QMdiArea; 
class QAction; 
class QMenu; 
class QSignalMapper;
class ReportChild;
class WinGeometry; 

class XArch : public QMainWindow
{
	Q_OBJECT

public:
	XArch(QWidget *parent = 0, Qt::WFlags flags = 0);
	~XArch();
public slots:
	void load(int patientId, int examId); 
protected:
     void closeEvent(QCloseEvent *event);
private:
	ReportChild* createReportChild(); 
	ReportChild* activeReportChild(); 
	void createActions(); 
	void createMenus(); 
private slots:
	void updateMenus(); 
	void updateWindowMenu(); 
	void setActiveSubWindow(QWidget*); 
	void on_newAct(); 
	void on_settingsAct(); 
	void on_testAct(); 
	void on_showDictsAct(); 
	void on_showTemplatesAct(); 
	void on_searchDbAct(); 
	void on_printAct(); 
private:
	WinGeometry* winGeometry; 
	QMdiArea* mdiArea; 
	QSignalMapper* windowMapper; 
	QMenu* windowMenu; 

	// actions
	QAction* newAct; 
	QAction* settingsAct; 
	QAction* testAct; 
	QAction* closeAct; 
	QAction* closeAllAct; 
	QAction* nextAct; 
	QAction* previousAct; 
	QAction* separatorAct; 
	QAction* showDictsAct; 
	QAction* showTemplatesAct; 
	QAction* searchDbAct;
	QAction* printAct; 
};

#endif // XARCH_H
