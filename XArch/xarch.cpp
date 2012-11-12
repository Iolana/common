#include <QtGui>
#include "xarch.h"
#include "patienteditor.h"
#include "wingeometry.h"
#include "exameditor.h"
#include "dictionaryeditor.h"
#include "templateseditor.h"
#include "searchdatabase.h"
#include "reportchild.h"
#include "dbadapter.h"
#include "printerdlg.h"
#include "Speller.h"
#include "FindDlg.h"
#include "DailyReportDlg.h"

XArch::XArch(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	setObjectName("MainWindow"); 
	mdiArea = new QMdiArea;
	mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setCentralWidget(mdiArea);
	connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(updateMenus()));
	windowMapper = new QSignalMapper(this); 
	connect(windowMapper, SIGNAL(mapped(QWidget*)), this, SLOT(setActiveSubWindow(QWidget*)));

	createActions(); 
	createMenus(); 
	
	winGeometry = new WinGeometry(this); 
	winGeometry->load(); 
	statusBar()->showMessage(tr("Application loaded")); 
	setWindowTitle(tr("XArch")); 
	new DbAdapter(this); 

    mSpeller = new Speller(this);
}

XArch::~XArch()
{
}
void XArch::closeEvent(QCloseEvent *event)
{
	mdiArea->closeAllSubWindows();
	if (mdiArea->currentSubWindow()) 
		event->ignore();
	else 
	{
		winGeometry->save(); 
		event->accept();
	}
}
void XArch::createActions()
 {
	newAct = new QAction(tr("&New..."), this); 
	newAct->setShortcut(QKeySequence(tr("Ctrl+N"))); 
	connect(newAct, SIGNAL(triggered()), this, SLOT(on_newAct())); 

	settingsAct = new QAction(tr("Se&ttings..."), this); 
	connect(settingsAct, SIGNAL(triggered()), this, SLOT(on_settingsAct())); 

	testAct = new QAction(tr("&Test"), this); 
	testAct->setShortcut(QKeySequence(tr("Ctrl+T"))); 
	connect(testAct, SIGNAL(triggered()), this, SLOT(on_testAct())); 

	 closeAct = new QAction(tr("Cl&ose"), this);
     connect(closeAct, SIGNAL(triggered()), mdiArea, SLOT(closeActiveSubWindow()));

     closeAllAct = new QAction(tr("Close &All"), this);
     connect(closeAllAct, SIGNAL(triggered()), mdiArea, SLOT(closeAllSubWindows()));

	 nextAct = new QAction(tr("Ne&xt"), this);
     nextAct->setShortcuts(QKeySequence::NextChild);
     connect(nextAct, SIGNAL(triggered()), mdiArea, SLOT(activateNextSubWindow()));

     previousAct = new QAction(tr("Pre&vious"), this);
     previousAct->setShortcuts(QKeySequence::PreviousChild);
     connect(previousAct, SIGNAL(triggered()), mdiArea, SLOT(activatePreviousSubWindow()));

	 separatorAct = new QAction(this);
     separatorAct->setSeparator(true);

	 showDictsAct = new QAction(tr("&Dictionaries..."), this); 
	 connect(showDictsAct, SIGNAL(triggered()), this, SLOT(on_showDictsAct())); 

	 showTemplatesAct = new QAction(tr("&Templates..."), this); 
	 connect(showTemplatesAct, SIGNAL(triggered()), this, SLOT(on_showTemplatesAct())); 

	 searchDbAct = new QAction(tr("Searc&h database..."), this); 
     searchDbAct->setShortcut(QKeySequence(tr("Ctrl+F"))); 
	 connect(searchDbAct, SIGNAL(triggered()), this, SLOT(on_searchDbAct())); 

	printAct = new QAction(tr("Print..."), this); 
	printAct->setShortcut(QKeySequence(tr("Ctrl+P"))); 
	connect(printAct, SIGNAL(triggered()), this, SLOT(on_printAct())); 
    
    dailyReportAct = new QAction(tr("Daily report..."), this); 
    connect(dailyReportAct, SIGNAL(triggered()), this, SLOT(on_dailyReportAct())); 
}
void XArch::createMenus()
{
	menuBar()->show();
	QMenu* fileMenu = menuBar()->addMenu(tr("&File")); 
	fileMenu->addAction(newAct); 
	fileMenu->addAction(settingsAct); 
	fileMenu->addAction(testAct); 
	fileMenu->addAction(showDictsAct); 
	fileMenu->addAction(showTemplatesAct); 
	fileMenu->addAction(searchDbAct); 
	fileMenu->addAction(printAct); 
	fileMenu->addSeparator(); 

	windowMenu = menuBar()->addMenu(tr("&Window"));
	updateWindowMenu();
	connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));
    
    QMenu* reportsMenu = menuBar()->addMenu(tr("&Reports")); 
    reportsMenu->addAction(dailyReportAct); 
}
void XArch::load(int patientId, int examId)
{
	Patient patient = DbAdapter::getPatient(patientId); 
	Examination examination; 
	if(examId == -1)
	{
		ExamEditor examEditor(this); 
		if(examEditor.exec() == QDialog::Rejected)
			return; 
		examination = examEditor.getExamination(); 
		if(examination.patientId == -1)
			examination.patientId = patient.id; 
		if(examination.id == -1)
			examination.id = DbAdapter::insertExamination(examination); 
		if(examination.id == -1)
		{
			QMessageBox::critical(this, tr("Error"), tr("examination.id is still -1")); 
			return; 
		}
	}
	else
		examination = DbAdapter::getExamination(examId); 
	ReportChild* child = createReportChild(); 
    if(child != 0)
    {
        if(child->load(patient, examination))
        {
            statusBar()->showMessage(tr("Patient %1, %2 loaded").arg(patient.name).arg(DbAdapter::regionById(examination.regionId)), 2000);
            child->showMaximized();
        }
        else
            child->close();
    }
}
void XArch::on_newAct()
{
	PatientEditor patEditor(this); 
	if(patEditor.exec() == QDialog::Rejected)
		return; 
	Patient patient = patEditor.getPatient();
	if(patient.id == -1)
		patient.id = DbAdapter::insertPatient(patient); 
	if(patient.id == -1)
	{
		QMessageBox::critical(this, tr("Error"), tr("patient.id is still -1")); 
		return; 
	}

	ExamEditor examEditor(this); 
	if(examEditor.exec() == QDialog::Rejected)
		return; 
	Examination examination = examEditor.getExamination(); 
	if(examination.patientId == -1)
		examination.patientId = patient.id; 
	if(examination.id == -1)
		examination.id = DbAdapter::insertExamination(examination); 
	if(examination.id == -1)
	{
		QMessageBox::critical(this, tr("Error"), tr("examination.id is still -1")); 
		return; 
	}
	
	ReportChild* child = createReportChild(); 
    if(child != 0)
    {
        if(child->load(patient, examination))
        {
            statusBar()->showMessage(tr("Patient %1, %2 loaded").arg(patient.name).arg(DbAdapter::regionById(examination.regionId)), 2000);
            child->showMaximized();
        }
        else
            child->close();
    }
}
void XArch::on_settingsAct()
{}
void XArch::on_testAct()
{
    FindDlg dlg; 
    dlg.exec(); 
}
void XArch::updateMenus()
{
	
}
void XArch::updateWindowMenu()
{
     windowMenu->clear();
     windowMenu->addAction(closeAct);
     windowMenu->addAction(closeAllAct);
     windowMenu->addSeparator();
     windowMenu->addAction(nextAct);
     windowMenu->addAction(previousAct);
     windowMenu->addAction(separatorAct);

     QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
     separatorAct->setVisible(!windows.isEmpty());

     for (int i = 0; i < windows.size(); ++i) 
	 {
         ReportChild *child = qobject_cast<ReportChild*>(windows.at(i)->widget());
         QString text;
         if (i < 9) 
             text = tr("&%1 %2").arg(i + 1).arg(child->userFriendlyTitle());
         else 
             text = tr("%1 %2").arg(i + 1).arg(child->userFriendlyTitle());
         
         QAction *action  = windowMenu->addAction(text);
         action->setCheckable(true);
         action ->setChecked(child == activeReportChild());
         connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
         windowMapper->setMapping(action, windows.at(i));
     }
}
void XArch::setActiveSubWindow(QWidget *window)
{
	if (!window)
		return;
	mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}
ReportChild* XArch::createReportChild()
{
    ReportChild* child = new ReportChild(this, mSpeller);
	mdiArea->addSubWindow(child); 
	return child; 
}
 ReportChild* XArch::activeReportChild()
 {
     if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
         return qobject_cast<ReportChild*>(activeSubWindow->widget());
     return 0;
 }
 void XArch::on_showDictsAct()
 {
	 DictionaryEditor dlg; 
	 dlg.exec(); 
 }
 void XArch::on_showTemplatesAct()
 {
	TemplatesEditor dlg; 
	dlg.exec(); 
 }
 void XArch::on_searchDbAct()
 {
	 SearchDatabase dlg; 
	 connect(&dlg, SIGNAL(selected(int, int)), this, SLOT(load(int, int))); 
	 dlg.exec(); 
 }
 void XArch::on_printAct()
 {
	if(activeReportChild())
	{
		Patient patient = activeReportChild()->getPatient(); 
		Examination exam = activeReportChild()->getExamination(); 
		PrinterDlg dlg; 
		dlg.load(patient, exam); 
		dlg.exec(); 
    }
 }
 
 void XArch::on_dailyReportAct()
 {
    DailyReportDlg dlg; 
    dlg.exec();
 }
