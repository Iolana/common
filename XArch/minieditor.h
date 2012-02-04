#ifndef MINIEDITOR_H
#define MINIEDITOR_H

#include <QDialog>
#include "ui_minieditor.h"

class WinGeometry; 

class MiniEditor : public QDialog, public Ui::MiniEditor
{
	Q_OBJECT

public:
	MiniEditor(QWidget *parent = 0);
	~MiniEditor();
	void setText(const QString& text); 
	QString getText(); 
private slots:
	void on_okAct(); 
private:
	WinGeometry* winGeometry; 
	QAction* okAct; 
};

#endif // MINIEDITOR_H
