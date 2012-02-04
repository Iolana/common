#ifndef SEARCHDATABASE_H
#define SEARCHDATABASE_H

#include <QDialog>
#include "ui_searchdatabase.h"

class QSqlRelationalTableModel; 
class QSqlTableModel; 
class WinGeometry; 

class SearchDatabase : public QDialog, public Ui::SearchDatabase
{
	Q_OBJECT

public:
	SearchDatabase(QWidget *parent = 0);
	~SearchDatabase();
signals:
	void selected(int patientId, int examId); 
private slots:
	void on_patientsTable_clicked(const QModelIndex& index); 
	void on_patientsTable_doubleClicked(const QModelIndex& index); 
	void on_examinationsTable_doubleClicked(const QModelIndex& index); 
	void on_btnSearch_clicked(); 
private:
	WinGeometry* winGeometry; 
	QSqlTableModel* patientsModel; 
	QSqlRelationalTableModel* examsModel; 
};

#endif // SEARCHDATABASE_H
