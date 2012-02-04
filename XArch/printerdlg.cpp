#include "printerdlg.h"
#include "wingeometry.h" 
#include "dbadapter.h"
#include <QFile>
#include <QTextStream>
#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPrintPreviewWidget>
#include <QFileDialog>
#include <QDomDocument>
#include <QMessageBox>

PrinterDlg::PrinterDlg(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	winGeometry = new WinGeometry(this); 
	winGeometry->load(); 
	printer = new QPrinter(QPrinter::HighResolution); 
	printer->setPageMargins(20.0, 15.0, 15.0, 15.0, QPrinter::Millimeter); 
	previewWidget = new QPrintPreviewWidget(printer, this); 
	int h = height() / 2; 
	int w = h * 0.71; 
	previewWidget->setMinimumSize(w, h); 
	previewWidget->setMaximumSize(w, h); 
	connect(previewWidget , SIGNAL(paintRequested(QPrinter*)), SLOT(paintRequested(QPrinter*)));
	connect(previewWidget , SIGNAL(previewChanged()), SLOT(on_previewWidget_previewChanged()));
	previewLayout->insertWidget(0, previewWidget); 
}

PrinterDlg::~PrinterDlg()
{
	winGeometry->save(); 
	delete printer; 
}
void PrinterDlg::load(const Patient& patient, const Examination& exam)
{
	QString fname = QString("%1/templates/%2")
		.arg(QApplication::applicationDirPath())
		.arg(DbAdapter::templateForModality(exam.modalityId)); 
	QFile file(fname);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	QTextStream in(&file);
	in.setCodec("UTF-8");
	QString templ = in.readAll(); 
	QList<std::pair<QString, QString> > dict; 
	//QHash<QString, QString> dict; 
	dict.append(std::make_pair("%patient_name%", patient.name)); 
	dict.append(std::make_pair("%patient_birthday%", patient.birthday.toString("dd.MM.yyyy"))); 
	dict.append(std::make_pair("%patient_birthyear%", patient.birthday.toString("yyyy"))); 
	//dict.append(std::make_pair("%patient_sex%", patient.sex == 0 ? tr("male") : tr("female"))); 
	dict.append(std::make_pair("%content%", parseContent(exam.content))); 
	dict.append(std::make_pair("%conclusion%", exam.conclusion)); 
	//dict.append(std::make_pair("%modality%", DbAdapter::modalityById(exam.modalityId))); 
	dict.append(std::make_pair("%device%", DbAdapter::deviceById(exam.deviceId))); 
	dict.append(std::make_pair("%region%", DbAdapter::regionById(exam.regionId))); 
	dict.append(std::make_pair("%contrast%", DbAdapter::contrastById(exam.contrastId))); 
	dict.append(std::make_pair("%contrast_amount%", exam.contrastAmount == 0 ? QString() : QString("%1ml").arg(exam.contrastAmount))); 
	dict.append(std::make_pair("%ordered_by%", DbAdapter::orderedByById(exam.orderedById))); 
	dict.append(std::make_pair("%exam_date%", exam.examDate.toString("dd.MM.yyyy"))); 

	for(int i = 0; i < dict.size(); ++i)
		templ.replace(dict.at(i).first, dict.at(i).second); 
		
	textEdit->setHtml(templ); 
}
void PrinterDlg::on_btnPrint_clicked()
{
    printer->setOutputFormat(QPrinter::NativeFormat);
    printer->setOutputFileName("");
    QPrintDialog *dialog = new QPrintDialog(printer, this);
    dialog->setWindowTitle(tr("Print Document"));
    if (dialog->exec() == QDialog::Accepted)
		textEdit->print(printer); 
}
void PrinterDlg::on_btnToPdf_clicked()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Export PDF"), QString(), "*.pdf");
    if (!fileName.isEmpty())
    {
        if (QFileInfo(fileName).suffix().isEmpty())
            fileName.append(".pdf");
        printer->setOutputFormat(QPrinter::PdfFormat);
        printer->setOutputFileName(fileName);
        textEdit->print(printer);
    }
}
void PrinterDlg::on_btnCancel_clicked()
{
	reject(); 
}
void PrinterDlg::paintRequested(QPrinter* p)
{
	textEdit->print(p); 
}
void PrinterDlg::on_textEdit_textChanged()
{
	previewWidget->updatePreview(); 
}
void PrinterDlg::on_previewWidget_previewChanged()
{
	pagesCnt->setText(tr("Pages %1").arg(previewWidget->pageCount())); 
}
void PrinterDlg::on_btnZoomIn_clicked()
{
	textEdit->setFontPointSize(textEdit->fontPointSize() + 0.5); 
	fontSize->setText(tr("Font size: %1").arg(textEdit->fontPointSize())); 
}
void PrinterDlg::on_btnZoomOut_clicked()
{
	textEdit->setFontPointSize(textEdit->fontPointSize() - 0.5); 
	fontSize->setText(tr("Font size: %1").arg(textEdit->fontPointSize())); 
}
void PrinterDlg::on_textEdit_selectionChanged()
{
	fontSize->setText(tr("Font size: %1").arg(textEdit->fontPointSize())); 
}
QString PrinterDlg::parseContent(const QString& content)
{
	QString text; 
	QTextStream out(&text); 
	QDomDocument doc; 
	QString errorStr;
	int errorLine;
	int errorColumn;
	if (!doc.setContent(content, true, &errorStr, &errorLine,
						&errorColumn)) 
	{
		QMessageBox::critical(0, tr("Error"),
						tr("Parse error at line %1, column %2:\n%3")
						.arg(errorLine)
						.arg(errorColumn)
						.arg(errorStr));
		return text;
	}
	QDomElement root = doc.documentElement(); 
	QDomElement item = root.firstChildElement("item");
	while (!item.isNull()) 
	{
		out << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"; 
		out << item.attribute("value"); 
		out << "<br />"; 
		item = item.nextSiblingElement("item");
	}
	return text; 
}