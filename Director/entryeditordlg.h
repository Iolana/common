#ifndef ENTRYEDITORDLG_H
#define ENTRYEDITORDLG_H

#include <QDialog>

namespace Ui {
class EntryEditorDlg;
}

class EntryEditorDlg : public QDialog
{
    Q_OBJECT

public:
    explicit EntryEditorDlg(QWidget *parent = 0);
    ~EntryEditorDlg();

private:
    Ui::EntryEditorDlg *ui;
};

#endif // ENTRYEDITORDLG_H
