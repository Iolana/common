#ifndef DAILYREPORTDLG_H
#define DAILYREPORTDLG_H

#include <QDialog>
#include "DbTableModel.h"

class WinGeometry; 

namespace Ui {
class DailyReportDlg;
}

class DailyReportDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit DailyReportDlg(QWidget *parent = 0);
    ~DailyReportDlg();
private slots:
    void on_btnShow_clicked(); 
private:
    void loadCombos(); 
    
private:
    Ui::DailyReportDlg *ui;
    DbTableModel mModel;
    WinGeometry* winGeometry; 
};

#endif // DAILYREPORTDLG_H
