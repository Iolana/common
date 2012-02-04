#include "searchdatabase.h"
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QMessageBox>
#include "wingeometry.h"

SearchDatabase::SearchDatabase(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	winGeometry = new WinGeometry(this); 
	winGeometry->load(); 
	examsModel = 0; 
	patientsModel = new QSqlTableModel(this);
	patientsModel->setTable("Patients");

	patientsModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
	patientsModel->select();
	patientsModel->setHeaderData(0, Qt::Horizontal, tr("Id"));
	patientsModel->setHeaderData(1, Qt::Horizontal, tr("Name"));
	patientsModel->setHeaderData(2, Qt::Horizontal, tr("Birthday"));
	patientsModel->setHeaderData(3, Qt::Horizontal, tr("Sex"));

	patientsTable->setModel(patientsModel); 
	patientsTable->resizeRowsToContents(); 
	patientsTable->resizeColumnsToContents(); 
}

SearchDatabase::~SearchDatabase()
{
	winGeometry->save(); 
}
void SearchDatabase::on_patientsTable_clicked(const QModelIndex& index)
{
	int patientId = index.sibling(index.row(), 0).data().toInt(); 
	if(examsModel != 0)
		delete examsModel; 
	examsModel = new QSqlRelationalTableModel(this);
	examsModel->setTable("Examinations");
	examsModel->setFilter(QString("patient = %1").arg(patientId)); 
	examsModel->setRelation(3, QSqlRelation("Modalities", "id", "name"));
	examsModel->setRelation(4, QSqlRelation("Devices", "id", "name"));
	examsModel->setRelation(5, QSqlRelation("Regions", "id", "name"));
	examsModel->setRelation(6, QSqlRelation("Contrasts", "id", "name"));
	examsModel->setRelation(8, QSqlRelation("OrderedBy", "id", "name"));

	examsModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
	examsModel->select();
	examsModel->removeColumn(9); 
	examsModel->removeColumn(8); 
	examsModel->removeColumn(7); 
	examsModel->removeColumn(4); 
	examsModel->removeColumn(1); 
	//examsModel->removeColumn(0); 	
	examsModel->setHeaderData(0, Qt::Horizontal, tr("Id")); 
	examsModel->setHeaderData(1, Qt::Horizontal, tr("Date")); 
	examsModel->setHeaderData(2, Qt::Horizontal, tr("Modality")); 
	examsModel->setHeaderData(3, Qt::Horizontal, tr("Region")); 
	examsModel->setHeaderData(4, Qt::Horizontal, tr("Contrast")); 
	examsModel->setHeaderData(5, Qt::Horizontal, tr("Conclusion")); 
	examinationsTable->setModel(examsModel); 
	examinationsTable->resizeRowsToContents(); 
	examinationsTable->resizeColumnsToContents(); 
}
void SearchDatabase::on_btnSearch_clicked()
{
	patientsModel->setFilter(QString("name like '%%%1%%'").arg(searchText->text())); 
	patientsModel->select(); 
}
void SearchDatabase::on_patientsTable_doubleClicked(const QModelIndex& index)
{
	int patientId = index.sibling(index.row(), 0).data().toInt(); 
	emit selected(patientId, -1); 
	close(); 
}
void SearchDatabase::on_examinationsTable_doubleClicked(const QModelIndex& index)
{
	if(patientsTable->selectionModel()->hasSelection())
	{
		int patientId = patientsModel->index(patientsTable->currentIndex().row(), 0).data().toInt(); 
		int examId = index.sibling(index.row(), 0).data().toInt(); 
		emit selected(patientId, examId); 
		close(); 
	}
}