#include "DailyReportDlg.h"
#include "ui_DailyReportDlg.h"
#include "dbadapter.h"
#include "wingeometry.h"

DailyReportDlg::DailyReportDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DailyReportDlg)
{
    ui->setupUi(this);
    winGeometry = new WinGeometry(this); 
	winGeometry->load(); 
    
    loadCombos();
}

DailyReportDlg::~DailyReportDlg()
{
    winGeometry->save(); 
    delete ui;
}

void DailyReportDlg::on_btnShow_clicked()
{
    QStringList cols; 
    cols << "Patient" << "Date" << "Region" << "Contrast" << "Conclusion"; 
    QList<QStringList> data = DbAdapter::findExamsByDateAndDevice(ui->examDate->date(), 
                                                                  ui->device->itemData(ui->device->currentIndex()).toInt()); 
    mModel.load(cols, data); 
    
    ui->tableView->setModel(&mModel);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();    
}

void DailyReportDlg::loadCombos()
{
    QHash<int, QString> list = DbAdapter::getAllDevices(-1); 
	QHash<int, QString>::const_iterator i = list.constBegin(); 
	while(i != list.constEnd())
	{
		ui->device->addItem(i.value(), i.key()); 
		++i; 
	}
}
