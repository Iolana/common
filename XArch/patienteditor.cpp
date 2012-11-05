#include "patienteditor.h"
#include "wingeometry.h"
#include "dbadapter.h"
#include "searchdatabase.h"

PatientEditor::PatientEditor(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	winGeometry = new WinGeometry(this); 
	winGeometry->load(); 
	QStringList list; 
	list << tr("Male") << tr("Female"); 
	sex->addItems(list); 
    
    createActions(); 
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

void PatientEditor::createActions()
{
    prevAct = new QAction(tr("&Previous patient"), this); 
	prevAct->setShortcut(QKeySequence(tr("Ctrl+N"))); 
	connect(prevAct, SIGNAL(triggered()), this, SLOT(on_btnPrev_clicked()));
    addAction(prevAct);
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

void PatientEditor::on_btnFind_clicked()
{
    SearchDatabase dlg; 
    connect(&dlg, SIGNAL(selected(int,int)), this, SLOT(found(int, int))); 
    dlg.exec(); 
}

void PatientEditor::on_btnPrev_clicked()
{
    setPatient(DbAdapter::getLatestPatient());
}

void PatientEditor::on_btnClear_clicked()
{
    Patient p; 
    p.birthday = QDate::fromString("01012000", "ddMMyyyy"); 
    p.sex = 0; 
    setPatient(p);
}
void PatientEditor::found(int patientId, int examId)
{
    Q_UNUSED(examId)
    setPatient(DbAdapter::getPatient(patientId));
}
