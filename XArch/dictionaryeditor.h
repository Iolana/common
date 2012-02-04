#ifndef DICTIONARYEDITOR_H
#define DICTIONARYEDITOR_H

#include <QDialog>
#include "ui_dictionaryeditor.h"

class IdTextModel; 
class WinGeometry; 

class DictionaryEditor : public QDialog, public Ui::DictionaryEditor
{
	Q_OBJECT

public:
	DictionaryEditor(QWidget *parent = 0);
	~DictionaryEditor();
private slots:
	void on_btnAdd_clicked(); 
	void on_btnDelete_clicked(); 
	void on_menu_currentIndexChanged(int index); 
	void on_modality_currentIndexChanged(const QString& text); 
	void on_listView_doubleClicked(const QModelIndex& index); 
private:
	void reload(); 
private:
	WinGeometry* winGeometry; 
	IdTextModel* model; 
};

#endif // DICTIONARYEDITOR_H
