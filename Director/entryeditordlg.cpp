#include "entryeditordlg.h"
#include "ui_entryeditordlg.h"

EntryEditorDlg::EntryEditorDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EntryEditorDlg)
{
    ui->setupUi(this);
}

EntryEditorDlg::~EntryEditorDlg()
{
    delete ui;
}
