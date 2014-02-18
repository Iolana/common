#ifndef NEWENTRYDLG_H
#define NEWENTRYDLG_H

#include <QDialog>

namespace Ui {
class NewEntryDlg;
}

class NewEntryDlg : public QDialog
{
    Q_OBJECT

public:
    explicit NewEntryDlg(QWidget *parent = 0);
    ~NewEntryDlg();
    void setFileMode(bool isFile);
    QString getName();
    QString getUuid();
    QString getTags();
    QString getStored();
    QString getComment();

private slots:
    void on_pathBtn_clicked();
    void on_commentBtn_clicked();

private:
    Ui::NewEntryDlg *ui;
    bool mIsFileMode;
};

#endif // NEWENTRYDLG_H
