#ifndef EDITORDLG_H
#define EDITORDLG_H

#include <QDialog>
#include "ui_editordlg.h"

class QContextMenuEvent; 
class WinGeometry; 
class QToolBar; 
class QStringListModel; 
class Speller;
class SpellHighlighter;

class EditorDlg : public QDialog, public Ui::EditorDlg
{
	Q_OBJECT

public:
	enum Mode
	{ REPORT_MODE, COMMON_REPORT_MODE, CONCLUSION_MODE }; 
    EditorDlg(QWidget *parent = 0, Speller* speller = 0);
	~EditorDlg();
	void setup(const QString& text, int templateId, int _regionId, Mode _mode); 
	QString getText(); 
	
private slots:
	void	on_listView_doubleClicked(const QModelIndex& index); 

	void on_okAct(); 
	void on_defaultAct(); 
	void on_addAct(); 
	void on_delAct(); 
	void on_constructorAct(); 
private:
	void createActions(); 
	void createToolbar(); 
	QString getDefaultPhrase(); 
	QStringList getPhrases(); 
	void updateListModel(); 
private:
    SpellHighlighter* mSpellHighlighter;
	Mode mode; 
	WinGeometry* winGeometry; 
	int templateItemId; 
	int regionId; 
	QToolBar* toolBar; 
	QAction* defaultAct; 
	QAction* addAct; 
	QAction* delAct; 
	QAction* okAct; 
	QStringListModel* listModel; 
};

#endif // EDITORDLG_H
