#define REFERRALS_IDX 0
#define DEVICES_IDX 1
#define REGIONS_IDX 2
#define CONTRASTS_IDX 3

#include "dictionaryeditor.h"
#include "dbadapter.h"
#include <QStringListModel>
#include <QMessageBox>
#include "idtextmodel.h"
#include "minieditor.h"
#include "wingeometry.h"

DictionaryEditor::DictionaryEditor(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	winGeometry = new WinGeometry(this); 
	winGeometry->load(); 
	model = new IdTextModel(this); 
	listView->setModel(model); 
	QStringList list; 
	list << tr("Referrals")
		<< tr("Devices")
		<< tr("Regions")
		<< tr("Contrasts"); 
	menu->insertItems(0, list); 

	QHash<int, QString> mods = DbAdapter::getAllModalities(); 
	QHash<int, QString>::const_iterator i = mods.constBegin(); 
	while(i !=mods.constEnd())
	{
		modality->addItem(i.value(), i.key()); 
		++i; 
	}
}

DictionaryEditor::~DictionaryEditor()
{
	winGeometry->save(); 
}
void DictionaryEditor::on_btnAdd_clicked()
{
	MiniEditor dlg; 
	if(dlg.exec() == QDialog::Accepted)
	{
		int mod = modality->itemData(modality->currentIndex()).toInt(); 
		switch(menu->currentIndex())
		{
		case REFERRALS_IDX:
			DbAdapter::insertReferral(dlg.getText()); 
			break; 
		case DEVICES_IDX:
			DbAdapter::insertDevice(dlg.getText(), mod); 
			break; 
		case REGIONS_IDX:
			DbAdapter::insertRegion(dlg.getText(), mod); 
			break; 
		case CONTRASTS_IDX:
			DbAdapter::insertContrast(dlg.getText(), mod); 
			break; 
		}
		reload(); 
	}
}
void DictionaryEditor::on_btnDelete_clicked()
{
	if(listView->selectionModel()->hasSelection())
	{
		int id = listView->currentIndex().data(Qt::UserRole).toInt(); 
		switch(menu->currentIndex())
		{
		case REFERRALS_IDX:
			DbAdapter::deleteReferral(id); 
			break; 
		case DEVICES_IDX:
			DbAdapter::deleteDevice(id); 
			break; 
		case REGIONS_IDX:
			DbAdapter::deleteRegion(id); 
			break; 
		case CONTRASTS_IDX:
			DbAdapter::deleteContrast(id); 
			break; 
		}
		reload(); 
	}
}
void DictionaryEditor::on_listView_doubleClicked(const QModelIndex& index)
{
	int id = index.data(Qt::UserRole).toInt(); 
	MiniEditor dlg; 
	dlg.setText(index.data().toString()); 
	if(dlg.exec() == QDialog::Accepted)
	{
		switch(menu->currentIndex())
		{
		case REFERRALS_IDX:
			DbAdapter::updateReferral(id, dlg.getText()); 
			break; 
		case DEVICES_IDX:
			DbAdapter::updateDevice(id, dlg.getText()); 
			break; 
		case REGIONS_IDX:
			DbAdapter::updateRegion(id, dlg.getText()); 
			break; 
		case CONTRASTS_IDX:
			DbAdapter::updateContrast(id, dlg.getText()); 
			break; 
		}
		reload(); 
	}
}
void DictionaryEditor::on_menu_currentIndexChanged(int index)
{
	if(index == REFERRALS_IDX)
		modality->hide(); 
	else if(modality->isHidden())
		modality->show(); 
	reload(); 
}
void DictionaryEditor::on_modality_currentIndexChanged(const QString& text)
{
	reload(); 
}
/*! Обновляет содержимое списка в соответствии с текущим выбором в menu
\param index индекс выбранного пункта в menu */
void DictionaryEditor::reload()
{
	int mod = modality->currentIndex(); 
	QHash<int, QString> hash; 
	switch(menu->currentIndex())
	{
	case REFERRALS_IDX:
		hash = DbAdapter::getAllOrders(); 
		break; 
	case DEVICES_IDX:
		hash = DbAdapter::getAllDevices(mod); 
		break; 
	case REGIONS_IDX:
		hash = DbAdapter::getAllRegions(mod); 
		break; 
	case CONTRASTS_IDX:
		hash = DbAdapter::getAllContrasts(mod); 
		break; 
	}
	model->setHash(hash); 
}
