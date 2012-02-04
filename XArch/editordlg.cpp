#include "editordlg.h"
#include <QtGui>
#include "dbadapter.h"
#include "wingeometry.h"

EditorDlg::EditorDlg(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this); 

	mode = REPORT_MODE; 
	templateItemId = -1; 
	regionId = -1; 
	listModel = new QStringListModel(this); 
	listView->setModel(listModel); 

	winGeometry = new WinGeometry(this); 
	winGeometry->load(); 

	createActions(); 
	createToolbar(); 
}
EditorDlg::~EditorDlg()
{
	winGeometry->save(); 
}
void EditorDlg::createActions()
{
	defaultAct = new QAction(tr("De&fault"), this); 
	connect(defaultAct, SIGNAL(triggered()), this, SLOT(on_defaultAct())); 

	addAct = new QAction(tr("&Add phrase"), this); 
	addAct->setShortcut(QKeySequence(tr("Ctrl+I"))); 
	connect(addAct, SIGNAL(triggered()), this, SLOT(on_addAct())); 

	delAct = new QAction(tr("&Delete phrase"), this); 
	delAct->setShortcut(QKeySequence(tr("Ctrl+D"))); 
	connect(delAct, SIGNAL(triggered()), this, SLOT(on_delAct())); 

	okAct = new QAction(tr("O&k"), this); 
	okAct->setShortcut(QKeySequence(tr("Ctrl+S"))); 
	connect(okAct, SIGNAL(triggered()), this, SLOT(on_okAct())); 
		
}
void EditorDlg::createToolbar()
{
	toolBar = new QToolBar(this); 
	verticalLayout->insertWidget(0, toolBar); 
	toolBar->addAction(defaultAct); 
	toolBar->addAction(addAct); 
	toolBar->addAction(delAct); 
	toolBar->addSeparator(); 
	toolBar->addAction(okAct); 
}
void EditorDlg::setup(const QString& text, int templateId, int _regionId, Mode _mode)
{
	textEdit->setText(text); 
	templateItemId = templateId; 
	regionId = _regionId; 
	mode = _mode; 
	updateListModel(); 
}
void EditorDlg::updateListModel()
{
	switch(mode)
	{
	case REPORT_MODE:
		if(templateItemId >= 0)
			listModel->setStringList(DbAdapter::getAltPhrases(templateItemId)); 
		else
			QMessageBox::critical(this, tr("Error"), tr("REPORT_MODE but templateItemId is negative")); 
		break; 
	case COMMON_REPORT_MODE:
		if(regionId >= 0 )
			listModel->setStringList(DbAdapter::getCommonPhrases(regionId)); 
		else
			QMessageBox::critical(this, tr("Error"), tr("COMMON_REPORT_MODE but regionId is negative")); 
		break; 
	case CONCLUSION_MODE:
		if(regionId >= 0 )
			listModel->setStringList(DbAdapter::getConclusionPhrases(regionId)); 
		else
			QMessageBox::critical(this, tr("Error"), tr("CONCLUSION_MODE but regionId is negative")); 
		break; 
	}		
}
QString EditorDlg::getText()
{
	return textEdit->toPlainText(); 
}
void EditorDlg::on_okAct()
{
	accept();
}
void EditorDlg::on_defaultAct()
{
	textEdit->clear(); 
	textEdit->setText(getDefaultPhrase()); 
}
void EditorDlg::on_addAct()
{
	QString text; 
	if(textEdit->textCursor().hasSelection())
		text = textEdit->textCursor().selectedText(); 
	else
		text = textEdit->toPlainText(); 

	switch(mode)
	{
	case REPORT_MODE:
		if(templateItemId >= 0)
			DbAdapter::addAltPhrase(templateItemId, text); 
		else
			QMessageBox::critical(this, tr("Error"), tr("REPORT_MODE but templateItemId is negative")); 
		break; 
	case COMMON_REPORT_MODE:
		if(regionId >= 0 )
			DbAdapter::addCommonPhrase(regionId, text); 
		else
			QMessageBox::critical(this, tr("Error"), tr("COMMON_REPORT_MODE but regionId is negative")); 
		break; 
	case CONCLUSION_MODE:
		if(regionId >= 0 )
			DbAdapter::addConclusionPhrase(regionId, text); 
		else
			QMessageBox::critical(this, tr("Error"), tr("CONCLUSION_MODE but regionId is negative")); 
		break; 
	}		

	updateListModel(); 
}
void EditorDlg::on_delAct()
{
	if(!listView->selectionModel()->hasSelection())
		return; 
	QString text = listView->currentIndex().data().toString(); 

	switch(mode)
	{
	case REPORT_MODE:
		if(templateItemId >= 0)
			DbAdapter::delAltPhrase(templateItemId, text); 
		else
			QMessageBox::critical(this, tr("Error"), tr("REPORT_MODE but templateItemId is negative")); 
		break; 
	case COMMON_REPORT_MODE:
		if(regionId >= 0 )
			DbAdapter::delCommonPhrase(regionId, text); 
		else
			QMessageBox::critical(this, tr("Error"), tr("COMMON_REPORT_MODE but regionId is negative")); 
		break; 
	case CONCLUSION_MODE:
		if(regionId >= 0 )
			DbAdapter::delConclusionPhrase(regionId, text); 
		else
			QMessageBox::critical(this, tr("Error"), tr("CONCLUSION_MODE but regionId is negative")); 
		break; 
	}

	updateListModel(); 
}
void EditorDlg::on_constructorAct()
{}
QString EditorDlg::getDefaultPhrase()
{
	switch(mode)
	{
	case REPORT_MODE:
		if(templateItemId >= 0)
			return DbAdapter::getTemplateItem(templateItemId); 
		else
			QMessageBox::critical(this, tr("Error"), tr("REPORT_MODE but templateItemId is negative")); 
		break; 
	case COMMON_REPORT_MODE:
		break; 
	case CONCLUSION_MODE:
		if(regionId >= 0 )
			return DbAdapter::getConclusionPhrase(regionId); 
		else
			QMessageBox::critical(this, tr("Error"), tr("CONCLUSION_MODE but regionId is negative")); 
		break; 
	}
	return QString(); 
}
void	EditorDlg::on_listView_doubleClicked(const QModelIndex& index)
{
	if(textEdit->textCursor().hasSelection())
		textEdit->textCursor().removeSelectedText(); 
	textEdit->insertPlainText(index.data().toString()); 
}

/*
void EditorDlg::on_phrasesAct()
{
	int maxLengh = 64; 
	QSignalMapper mapper; 
	connect(&mapper, SIGNAL(mapped(const QString&)), this, SLOT(on_phraseMapped(const QString&)));
	QMenu menu(this);
	QStringList phrases = getPhrases(); 
	if(phrases.size() == 0)
		return; 
	for(int i = 0; i < phrases.size(); ++i)
	{
		QString curPhrase = phrases.at(i); ; 
		QString text;
        if (i < 9) 
			text = tr("&%1 %2").arg(i + 1).arg(curPhrase.length() < maxLengh ? curPhrase : curPhrase.mid(0, maxLengh));
		else 
			text = tr("%1 %2").arg(i + 1).arg(curPhrase.length() < maxLengh ? curPhrase : curPhrase.mid(0, maxLengh));
		QAction* act = menu.addAction(text); 
		connect(act, SIGNAL(triggered()), &mapper, SLOT(map()));
		mapper.setMapping(act, curPhrase);
	}
	int x = width() / 2 - menu.width() / 2; 
	int y = height() / 2 - menu.height() / 2; 
	menu.exec(mapToGlobal(QPoint(x, y)));
}*/