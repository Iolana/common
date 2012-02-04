#ifndef EXAMEDITOR_H
#define EXAMEDITOR_H

#include <QDialog>
#include "ui_exameditor.h"
#include "structures.h"

class WinGeometry; 

class ExamEditor : public QDialog, public Ui::ExamEditor
{
	Q_OBJECT

public:
	ExamEditor(QWidget *parent = 0);
	~ExamEditor();
	void setExamination(const Examination& e); 
	Examination getExamination(); 
private:
	void loadCombos(); 
	int currentData(QComboBox* combo); 
	void setCurrentData(QComboBox* combo, int id); 
private slots:
	void on_btnOk_clicked(); 
	void on_btnCancel_clicked(); 
	void on_modality_currentIndexChanged(int index); 
private:
	WinGeometry* winGeometry; 
	Examination exam; 
};

#endif // EXAMEDITOR_H
