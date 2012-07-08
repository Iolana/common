#ifndef PATIENTEDITOR_H
#define PATIENTEDITOR_H

#include <QDialog>
#include "ui_patienteditor.h"
#include "structures.h"

class WinGeometry; 
class QAction;

class PatientEditor : public QDialog, public Ui::PatientEditor
{
	Q_OBJECT

public:
	PatientEditor(QWidget *parent = 0);
	~PatientEditor();
	void setPatient(const Patient& p); 
	Patient getPatient(); 
private slots:
	void on_btnOk_clicked(); 
	void on_btnCancel_clicked(); 
    void on_btnFind_clicked(); 
    void on_btnPrev_clicked(); 
    void on_btnClear_clicked(); 
    void found(int patientId, int examId); 
private:
	WinGeometry* winGeometry; 
	Patient patient; 
};

#endif // PATIENTEDITOR_H
