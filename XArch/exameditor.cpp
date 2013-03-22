#include "exameditor.h"
#include <QMessageBox>
#include "dbadapter.h"
#include "wingeometry.h"

ExamEditor::ExamEditor(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	winGeometry = new WinGeometry(this); 
	winGeometry->load(); 
	QHash<int, QString> list = DbAdapter::getAllModalities(); 
	if(list.size() == 0)
		QMessageBox::critical(this, tr("Error"), tr("Modalities list is empty")); 
	QHash<int, QString>::const_iterator i = list.constBegin(); 
	while(i != list.constEnd())
	{
		modality->addItem(i.value(), i.key()); 
		++i; 
	}

	list = DbAdapter::getAllOrders(); 
	if(list.size() == 0)
		QMessageBox::critical(this, tr("Error"), tr("OrderBy list is empty")); 
	i = list.constBegin(); 
	while(i != list.constEnd())
	{
		orderedBy->addItem(i.value(), i.key()); 
		++i; 
	}
	loadCombos(); 
	examDate->setDate(QDate::currentDate()); 
    quantity->setValue(1);
    cost->setText("0");
}

ExamEditor::~ExamEditor()
{
	winGeometry->save(); 
}
void ExamEditor::setExamination(const Examination& e)
{
	exam = e; 
	setCurrentData(modality, exam.modalityId); 
	setCurrentData(device, exam.deviceId); 
	setCurrentData(region, exam.regionId); 
	setCurrentData(contrast, exam.contrastId); 
	contrastAmount->setValue(exam.contrastAmount); 
	setCurrentData(orderedBy, exam.orderedById); 
	examDate->setDate(exam.examDate); 
    quantity->setValue(exam.quantity);
    cost->setText(QString::number(exam.cost));
}
Examination ExamEditor::getExamination()
{
	return exam; 
}
void ExamEditor::on_btnOk_clicked()
{
	exam.modalityId = currentData(modality); 
	exam.deviceId = currentData(device); 
	exam.regionId = currentData(region); 
	exam.contrastId = currentData(contrast); 
	exam.contrastAmount = contrastAmount->value(); 
	exam.orderedById = currentData(orderedBy); 
	exam.examDate = examDate->date(); 
    exam.quantity = quantity->value();
    exam.cost = cost->text().toInt(); //TODO: make less error prone
	accept(); 
}
void ExamEditor::on_btnCancel_clicked()
{
	reject(); 
}
void ExamEditor::loadCombos()
{
	device->clear(); 
	region->clear(); 
	contrast->clear(); 
	int modalityId = currentData(modality); 

	QHash<int, QString> list = DbAdapter::getAllDevices(modalityId); 
	if(list.size() == 0)
		QMessageBox::critical(this, tr("Error"), tr("Devices list is empty")); 
	QHash<int, QString>::const_iterator i = list.constBegin(); 
	while(i != list.constEnd())
	{
		device->addItem(i.value(), i.key()); 
		++i; 
	}

	list = DbAdapter::getAllRegions(modalityId); 
	if(list.size() == 0)
		QMessageBox::critical(this, tr("Error"), tr("Regions list is empty")); 
	i = list.constBegin(); 
	while(i != list.constEnd())
	{
		region->addItem(i.value(), i.key()); 
		++i; 
	}

	list = DbAdapter::getAllContrasts(modalityId); 
	if(list.size() == 0)
		QMessageBox::critical(this, tr("Error"), tr("Contrasts list is empty")); 
	i = list.constBegin(); 
	while(i != list.constEnd())
	{
		contrast->addItem(i.value(), i.key()); 
		++i; 
	}
}
void ExamEditor::on_modality_currentIndexChanged(int index)
{
	loadCombos(); 
}
int ExamEditor::currentData(QComboBox* combo)
{
	if(combo->currentIndex() == -1)
	{
		QMessageBox::critical(this, tr("Error"), tr("Current index for %1 is -1").arg(combo->objectName())); 
		return -1; 
	}
	return combo->itemData(combo->currentIndex()).toInt(); 
}
void ExamEditor::setCurrentData(QComboBox* combo, int id)
{
	int index = combo->findData(id); 
	if(index == -1)
	{
		QMessageBox::critical(this, tr("Error"), tr("Cant find item in %1 with given id %2").arg(combo->objectName()).arg(id)); 
		return; 
	}
	combo->setCurrentIndex(index); 
}
//TODO: null and disable contrast amount if contrast media is 'none'
