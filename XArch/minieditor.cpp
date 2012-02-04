#include "minieditor.h"
#include <QToolBar> 
#include "wingeometry.h"

MiniEditor::MiniEditor(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	winGeometry  = new WinGeometry(this); 
	winGeometry->load(); 
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	okAct = new QAction(tr("O&k"), this); 
	QList<QKeySequence> list; 
	list << QKeySequence(tr("F2")) 
		<< QKeySequence(tr("Ctrl+S")); 
	okAct->setShortcuts(list); 
	connect(okAct, SIGNAL(triggered()), this, SLOT(on_okAct())); 

	QToolBar* bar = new QToolBar(this); 
	bar->addAction(okAct); 
	qobject_cast<QBoxLayout*>(layout())->insertWidget(0, bar); 
}
void MiniEditor::on_okAct()
{
	accept();
}
MiniEditor::~MiniEditor()
{
	winGeometry->save(); 
}
void MiniEditor::setText(const QString& text)
{
	textEdit->setText(text); 
}
QString MiniEditor::getText()
{
	return textEdit->toPlainText(); 
}