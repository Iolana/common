#include "templateseditor.h"
#include "wingeometry.h"
#include <QMessageBox>
#include <QDomDocument>
#include <QFileDialog>
#include <QSqlTableModel>
#include "dbadapter.h"
#include "minieditor.h"

TemplatesEditor::TemplatesEditor(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	winGeometry = new WinGeometry(this); 
	winGeometry->load(); 
	tableModel = new QSqlTableModel(this); 
	tableModel->setTable("TemplateItems"); 
	tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit); 
	tableView->setModel(tableModel); 

	altTableModel = new QSqlTableModel(this); 
	altTableModel->setTable("AltTemplateItems"); 
	tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit); 
	tableViewAlt->setModel(altTableModel); 

	QHash<int, QString> list = DbAdapter::getAllModalities(); 
	if(list.size() == 0)
		QMessageBox::critical(this, tr("Error"), tr("Modalities list is empty")); 
	QHash<int, QString>::const_iterator i = list.constBegin(); 
	while(i != list.constEnd())
	{
		modality->addItem(i.value(), i.key()); 
		++i; 
	}
	loadCombos(); 
}

TemplatesEditor::~TemplatesEditor()
{
	winGeometry->save(); 
}
void TemplatesEditor::loadCombos()
{
	region->clear(); 
	int modalityId = currentData(modality); 

	QHash<int, QString> list = DbAdapter::getAllRegions(modalityId); 
	if(list.size() == 0)
		QMessageBox::critical(this, tr("Error"), tr("Regions list is empty")); 
	QHash<int, QString>::const_iterator i = list.constBegin(); 
	i = list.constBegin(); 
	while(i != list.constEnd())
	{
		region->addItem(i.value(), i.key()); 
		++i; 
	}
}
void TemplatesEditor::loadTableView()
{
	tableModel->setFilter(QString("regionId = %1").arg(currentData(region))); 
	tableModel->setSort(2, Qt::AscendingOrder); 
	tableModel->select(); 
	//tableModel->removeColumn(1); 
	/*tableModel->setHeaderData(0, Qt::Horizontal, tr("ID")); 
	tableModel->setHeaderData(1, Qt::Horizontal, tr("Text")); */
	tableView->resizeColumnsToContents(); 
	//tableView->setColumnWidth(1, 400); 
}
int TemplatesEditor::currentData(QComboBox* combo)
{
	if(combo->currentIndex() == -1)
	{
		QMessageBox::critical(this, tr("Error"), tr("Current index for %1 is -1").arg(combo->objectName())); 
		return -1; 
	}
	return combo->itemData(combo->currentIndex()).toInt(); 
}
void TemplatesEditor::on_modality_currentIndexChanged(int index)
{
	if(index == -1)
		return; 
	loadCombos(); 
}
void TemplatesEditor::on_region_currentIndexChanged(int index)
{
	if(index == -1)
		return; 
	loadTableView(); 
}
void TemplatesEditor::on_btnInsert_clicked()
{
	int row = tableView->selectionModel()->hasSelection() ? 
		row = tableView->currentIndex().row() :
		row = tableModel->rowCount(); 
	for(int i = row; i < tableModel->rowCount(); ++i)
		tableModel->setData(tableModel->index(i, 2), i + 1); 

	tableModel->insertRow(row); 
	tableModel->setData(tableModel->index(row, 1), currentData(region)); 
	tableModel->setData(tableModel->index(row, 2), row); 
	tableModel->setData(tableModel->index(row, 3), tr("New text here...")); 
	tableModel->submitAll(); 
}
void TemplatesEditor::on_btnRemove_clicked()
{
	if(tableView->selectionModel()->hasSelection())
	{
		int row = tableView->currentIndex().row(); 
		tableModel->removeRow(row); 
		for(int i = row; i < tableModel->rowCount(); ++i)
			tableModel->setData(tableModel->index(i, 2), i - 1); 

		tableModel->submitAll(); 
	}
}
void TemplatesEditor::on_btnUp_clicked()
{
	if(tableView->selectionModel()->hasSelection())
	{
		int row = tableView->currentIndex().row(); 
		up(row); 
	}
}
void TemplatesEditor::up(int row)
{
	if(row > 0 && row < tableModel->rowCount())
	{
		tableModel->setData(tableModel->index(row - 1, 2), 
			tableModel->data(tableModel->index(row - 1, 2)).toInt() + 1); // опустить предыдущий
		tableModel->setData(tableModel->index(row, 2), 
			tableModel->data(tableModel->index(row, 2)).toInt() - 1); // поднять текущий
		tableModel->submitAll(); 
	}
}
void TemplatesEditor::on_btnDown_clicked()
{
	if(tableView->selectionModel()->hasSelection())
	{
		int row = tableView->currentIndex().row(); 
		up(row + 1); 
	}
}
void TemplatesEditor::on_tableView_doubleClicked(const QModelIndex& index)
{
	int textIndex = 3; 
	int row = index.row(); 
	QString text = index.sibling(row, textIndex).data().toString(); 
	MiniEditor dlg; 
	dlg.setText(text); 
	if(dlg.exec() == QDialog::Accepted)
	{
		tableModel->setData(index.sibling(row, textIndex), dlg.getText()); 
		tableModel->submitAll(); 
	}
}
void TemplatesEditor::on_tableView_clicked(const QModelIndex& index)
{
	int parentId = index.sibling(index.row(), 0).data().toInt(); 
	tableViewAlt->setProperty("parentId", parentId); 
	altTableModel->setFilter(QString("parentId = %1").arg(parentId)); 
	altTableModel->select(); 
	tableViewAlt->resizeColumnsToContents(); 
}
void TemplatesEditor::on_tableViewAlt_doubleClicked(const QModelIndex& index)
{
	int textIndex = 2; 
	int row = index.row(); 
	QString text = index.sibling(row, textIndex).data().toString(); 
	MiniEditor dlg; 
	dlg.setText(text); 
	if(dlg.exec() == QDialog::Accepted)
	{
		altTableModel->setData(index.sibling(row, textIndex), dlg.getText()); 
		altTableModel->submitAll(); 
		tableViewAlt->resizeColumnsToContents(); 
	}
}
void TemplatesEditor::on_btnAltInsert_clicked()
{
	int row = altTableModel->rowCount(); 

	altTableModel->insertRow(row); 
	altTableModel->setData(tableModel->index(row, 1), tableViewAlt->property("parentId").toInt()); 
	altTableModel->setData(tableModel->index(row, 2), tr("New text here...")); 
	altTableModel->submitAll(); 
}
void TemplatesEditor::on_btnAltRemove_clicked()
{
	if(tableViewAlt->selectionModel()->hasSelection())
		altTableModel->removeRow(tableViewAlt->currentIndex().row()); 
}
void TemplatesEditor::on_btnExport_clicked()
{
	QDomDocument doc("templates");
	QDomElement root = doc.createElement("templates");
	doc.appendChild(root);

	QList<int> mods = DbAdapter::getAllModalities().keys(); 
	foreach(int modId, mods)
	{
		QList<int> regions = DbAdapter::getAllRegions(modId).keys(); 
		foreach(int regionId, regions)
		{
			//write regionId
			QDomElement regionTag = doc.createElement("region");
			regionTag.setAttribute("id", regionId); 
			root.appendChild(regionTag);

			QList<std::pair<int, QString> > ts = DbAdapter::loadTemplate(regionId); 
			for(int i = 0; i < ts.size(); ++i)
			{
				//write template item
				QDomElement itemTag = doc.createElement("item");
				itemTag.setAttribute("index", i); 
				itemTag.setAttribute("value", ts.at(i).second); 
				regionTag.appendChild(itemTag);

				QStringList altList = DbAdapter::getAltPhrases(ts.at(i).first); 
				foreach(QString alt, altList)
				{
					QDomElement altTag = doc.createElement("alt");
					altTag.setAttribute("value", alt); 
					itemTag.appendChild(altTag);
				}
			}
		}
	}
	QString fileName = QFileDialog::getSaveFileName(this, tr("Export to file..."), 
		QCoreApplication::applicationDirPath(), 
		tr("XML files (*.xml)")); 
	QFile file(fileName); 
	if(!file.open(QIODevice::WriteOnly))
	{
		QMessageBox::critical(this, tr("Error"), tr("Unable to export.")); 
		return; 
	}
	file.write(doc.toByteArray()); 
	file.close(); 
}
