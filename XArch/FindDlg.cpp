#include "FindDlg.h"
#include "ui_FindDlg.h"
#include "dbadapter.h"

FindDlg::FindDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDlg)
{
    ui->setupUi(this);
    QStringList cols; 
    cols << "Id" << "Patient" << "Date" << "Device" << "Region" << "Contrast" << "Conclusion"; 
    QList<QStringList> data = DbAdapter::findExams(QDate(2012, 9, 10), QDate(2012, 9, 10)); 
    mModel.load(cols, data); 
    
    ui->tableView->setModel(&mModel);
    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();
}

FindDlg::~FindDlg()
{
    delete ui;
}
