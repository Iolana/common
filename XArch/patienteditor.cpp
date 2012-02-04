#include "patienteditor.h"
#include "wingeometry.h"

PatientEditor::PatientEditor(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	winGeometry = new WinGeometry(this); 
	winGeometry->load(); 
	QStringList list; 
	list << tr("Male") << tr("Female"); 
	sex->addItems(list); 
}

PatientEditor::~PatientEditor()
{
	winGeometry->save(); 
}
void PatientEditor::setPatient(const Patient& p)
{
	patient = p; 
	name->setText(patient.name); 
	birthday->setDate(patient.birthday); 
	sex->setCurrentIndex(patient.sex); 
}
Patient PatientEditor::getPatient()
{
	return patient; 
}
void PatientEditor::on_btnOk_clicked()
{
	patient.name = name->text(); 
	patient.birthday = birthday->date(); 
	patient.sex = sex->currentIndex(); 
	accept(); 
}
void PatientEditor::on_btnCancel_clicked()
{
	reject(); 
}