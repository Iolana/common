#ifndef PRINTERDLG_H
#define PRINTERDLG_H

#include <QDialog>
#include "ui_printerdlg.h"
#include "structures.h"

class QPrinter; 
class QPrintPreviewWidget; 
class WinGeometry; 

class PrinterDlg : public QDialog, public Ui::PrinterDlg
{
	Q_OBJECT

public:
	PrinterDlg(QWidget *parent = 0);
	~PrinterDlg();
	void load(const Patient& patient, const Examination& exam); 
private slots:
	void on_btnPrint_clicked(); 
	void on_btnToPdf_clicked(); 
	void on_btnCancel_clicked(); 
	void on_btnZoomIn_clicked(); 
	void on_btnZoomOut_clicked(); 
	void paintRequested(QPrinter* p); 
	void on_textEdit_textChanged(); 
	void on_textEdit_selectionChanged(); 
	void on_previewWidget_previewChanged(); 
private:
	static QString parseContent(const QString& content); 
private:
	WinGeometry* winGeometry; 
	QPrinter* printer; 
	QPrintPreviewWidget* previewWidget; 
};

#endif // PRINTERDLG_H
