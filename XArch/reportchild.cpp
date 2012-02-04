#include "reportchild.h"
#include <QMessageBox>
#include "reportmodel.h"
#include <QCloseEvent>
#include "dbadapter.h"
#include "patienteditor.h"
#include "exameditor.h"
#include "editordlg.h"

ReportChild::ReportChild(QWidget *parent)
	: QWidget(parent)
{
	saved = false; 
	setAttribute(Qt::WA_DeleteOnClose);
	setupUi(this);
	reportModel = new ReportModel(this); 
	reportList->setModel(reportModel); 
}

ReportChild::~ReportChild()
{

}
bool ReportChild::load(const Patient& p, const Examination& e)
{
	patient = p; 
	exam = e; 
	setWindowTitle(userFriendlyTitle()); 
	setHeader(); 
	reportModel->load(exam.regionId, exam.content); 
	if(exam.conclusion.isNull() || exam.conclusion.isEmpty())
		; // load from templates
	else
		conclusionText->setText(exam.conclusion); 
	return true; 
}
QString ReportChild::userFriendlyTitle()
{
	return QString("%1 - %2").arg(patient.name).arg(DbAdapter::regionById(exam.regionId)); 
}
void ReportChild::on_patientHeader_linkActivated(const QString& link)
{
	PatientEditor dlg; 
	dlg.setPatient(patient); 
	if(dlg.exec() == QDialog::Accepted)
	{
		patient = dlg.getPatient(); 
		DbAdapter::updatePatient(patient); 
		setHeader(); 
	}
}
void ReportChild::on_examHeader_linkActivated(const QString& link)
{
	ExamEditor dlg; 
	dlg.setExamination(exam); 
	if(dlg.exec() == QDialog::Accepted)
	{
		exam = dlg.getExamination(); 
		DbAdapter::updateExamination(exam); 
		setHeader(); 
	}
}
void ReportChild::setHeader()
{
		QString patTempl = QString(tr("<b>%1</b> %2 (%3) - %4 <a href='1'>change...</a>"))
		.arg(patient.name)
		.arg(patient.birthday.toString("dd.MM.yyyy"))
		.arg(QDate::currentDate().year() - patient.birthday.year())
		.arg(patient.sex == 0 ? "M" : "F"); 
	patientHeader->setText(patTempl); 

	QString examTempl = QString(tr("%1 (%2):<br/><span style='color:darkgreen'>%3 [%4%5]</span><br/><b>%6</b> OrderBy: %7 <a href='1'>change...</a>"))
		.arg(DbAdapter::modalityById(exam.modalityId))
		.arg(DbAdapter::deviceById(exam.deviceId))
		.arg(DbAdapter::regionById(exam.regionId))
		.arg(DbAdapter::contrastById(exam.contrastId))
		.arg(exam.contrastId == 0 ? QString() : QString(tr(" %1ml")).arg(exam.contrastAmount))
		.arg(exam.examDate.toString("dd.MM.yyyy"))
		.arg(DbAdapter::orderedByById(exam.orderedById)); 
	examHeader->setText(examTempl); 
}
void ReportChild::save()
{
	createContent(); 
	exam.conclusion = conclusionText->toPlainText(); 
	DbAdapter::updateReport(exam.id, exam.content); 
	DbAdapter::updateConclusion(exam.id, exam.conclusion); 
	saved = true; 
}
Patient ReportChild::getPatient()
{
	if(!saved)
		save(); 
	return patient; 
}
Examination ReportChild::getExamination()
{
	if(!saved)
		save(); 
	return exam; 
}
void ReportChild::on_conclusionText_textChanged()
{
	saved = false; 
}
void ReportChild::createContent() 
{
	exam.content = reportModel->content(); 
}
void ReportChild::closeEvent(QCloseEvent* e)
{
	save(); 
	e->accept(); 
}
void ReportChild::on_reportList_doubleClicked(const QModelIndex& index)
{
	editReportItem(index); 
}
void ReportChild::editReportItem(const QModelIndex& index)
{
	EditorDlg editor; 
    int itemId = index.data(Qt::UserRole).toInt();
    EditorDlg::Mode mode;
    if(itemId == -1)
        mode = EditorDlg::COMMON_REPORT_MODE;
    else
        mode = EditorDlg::REPORT_MODE;

    editor.setup(index.data().toString(), itemId, exam.regionId, mode);
	if(editor.exec() == QDialog::Accepted)
		reportModel->setData(index, editor.getText()); 
}
void ReportChild::on_btnAdd_clicked()
{
	int row = -1; 
	if(reportList->selectionModel()->hasSelection())
		row = reportList->currentIndex().row(); 
	else
		row = reportModel->rowCount();  
	EditorDlg editor; 
    editor.setup(QString(), -1, exam.regionId, EditorDlg::COMMON_REPORT_MODE); //сейчас только общаяя строка может быть добавлена
    //TODO: обдумать добавление стандартных строк
	if(editor.exec() == QDialog::Accepted)
	{
		reportModel->insertRow(row); 
		reportModel->setData(reportModel->index(row), editor.getText()); 
		reportModel->setData(reportModel->index(row), -1, Qt::UserRole); 
	}
}
void ReportChild::on_btnDelete_clicked()
{
	if(reportList->selectionModel()->hasSelection())
		reportModel->removeRow(reportList->currentIndex().row()); 
}
void ReportChild::on_btnPhrases_clicked()
{
	
}
