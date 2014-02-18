#include <QDebug>
#include <QSettings>
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>
#include "newentrydlg.h"
#include "ui_newentrydlg.h"

NewEntryDlg::NewEntryDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewEntryDlg)
{
    ui->setupUi(this);
    mIsFileMode = true;
}

NewEntryDlg::~NewEntryDlg()
{
    delete ui;
}

void NewEntryDlg::setFileMode(bool isFile)
{
    mIsFileMode = isFile;
    ui->commentBtn->setVisible(!mIsFileMode);
}

QString NewEntryDlg::getName()
{
    return ui->nameEdit->text();
}

QString NewEntryDlg::getUuid()
{
    return ui->uuidEdit->text();
}

QString NewEntryDlg::getTags()
{
    return ui->tagsEdit->text();
}

QString NewEntryDlg::getStored()
{
    return ui->storedEdit->text();
}

QString NewEntryDlg::getComment()
{
    return ui->commentEdit->toPlainText();
}

void NewEntryDlg::on_pathBtn_clicked()
{
    QSettings settings("ginasoft", "director",this);
    QString lastDir = settings.value("NewEntryDlg/LastDir", qApp->applicationDirPath()).toString();
    QString gpgDir = settings.value("GPG/dir", "/home/visa/").toString();
    QString path;
    if(mIsFileMode)
        path = QFileDialog::getOpenFileName(this, tr("Select file"), lastDir);
    else
        path = QFileDialog::getExistingDirectory(this, tr("Select folder"), lastDir);
    if(path.isEmpty())
        return;
    QFileInfo fileInfo(path);
    if(fileInfo.isFile()) {
        ui->nameEdit->setText(fileInfo.fileName());
        settings.setValue("NewEntryDlg/LastDir", fileInfo.absolutePath());
    }
    else if(fileInfo.isDir()){
        ui->nameEdit->setText(QDir(path).dirName());
        settings.setValue("NewEntryDlg/LastDir", fileInfo.absolutePath());
    }

    ui->pathEdit->setText(path);
    QProcess uuidgen;
    uuidgen.start("uuidgen");
    if(!uuidgen.waitForFinished()) {
        qDebug() << "WARNING: " << __FILE__ << ":" << __LINE__;
        return;
    }
    QByteArray uuidBytes = uuidgen.readAll();
    ui->uuidEdit->setText(QString(uuidBytes).trimmed());

    // encode
    QProcess gpg;
    QStringList arguments;
    arguments << "--trust-model=always";
    arguments << "--yes";
    arguments << "-e";
    arguments << QString("-r%1").arg("D94F8F25");
    arguments << QString("-o%1%2.gpg").arg(gpgDir).arg(ui->uuidEdit->text());
    arguments << path;
    gpg.start("gpg", arguments);
    if(!gpg.waitForFinished()) {
        qDebug() << "WARNING: " << __FILE__ << ":" << __LINE__;
        return;
    }
    QByteArray gpgBytes = gpg.readAll();
    QMessageBox::information(this, tr("Information"), tr("GPG finished."));
}

void NewEntryDlg::on_commentBtn_clicked()
{

}
