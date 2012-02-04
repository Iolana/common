#ifndef TEMPLATESEDITOR_H
#define TEMPLATESEDITOR_H

#include <QDialog>
#include "ui_templateseditor.h"

class QSqlTableModel; 
class WinGeometry; 

class TemplatesEditor : public QDialog, public Ui::TemplatesEditor
{
	Q_OBJECT

public:
	TemplatesEditor(QWidget *parent = 0);
	~TemplatesEditor();
private:
	void loadCombos(); 
	void loadTableView(); 
	int currentData(QComboBox* combo); 
private slots:
	void on_tableView_doubleClicked(const QModelIndex& index); 
	void on_tableViewAlt_doubleClicked(const QModelIndex& index); 
	void on_tableView_clicked(const QModelIndex& index); 
	void on_modality_currentIndexChanged(int index); 
	void on_region_currentIndexChanged(int index); 
	void on_btnInsert_clicked(); 
	void on_btnRemove_clicked(); 
	void on_btnAltInsert_clicked(); 
	void on_btnAltRemove_clicked(); 
	void on_btnUp_clicked(); 
	void on_btnDown_clicked(); 
	void on_btnExport_clicked(); 
private:
	void up(int row); 
private:
	WinGeometry* winGeometry; 
	QSqlTableModel* tableModel; 
	QSqlTableModel* altTableModel; 
};

#endif // TEMPLATESEDITOR_H
