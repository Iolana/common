#include <QDebug>
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

void EntryEditorDlg::setName(const QString &name)
{
    ui->nameEdit->setText(name);
}

void EntryEditorDlg::setUuid(const QString &uuid)
{
    ui->uuidEdit->setText(uuid);
}

void EntryEditorDlg::setTags(const QString &tags)
{
    ui->tagsEdit->setText(tags);
}

void EntryEditorDlg::setStored(const QString &stored)
{
    ui->storedEdit->setText(stored);
}

void EntryEditorDlg::setComment(const QString &comment)
{
    ui->commentEdit->setPlainText(comment);
}

QString EntryEditorDlg::getName()
{
    return ui->nameEdit->text();
}

QString EntryEditorDlg::getUuid()
{
    return ui->uuidEdit->text();
}

QString EntryEditorDlg::getTags()
{
    return ui->tagsEdit->text();
}

QString EntryEditorDlg::getStored()
{
    return ui->storedEdit->text();
}

QString EntryEditorDlg::getComment()
{
    return ui->commentEdit->toPlainText();
}
