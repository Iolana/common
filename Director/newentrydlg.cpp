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
    if(mIsFileMode)
        encryptFile();
    else
        encryptDir();
}

void NewEntryDlg::on_commentBtn_clicked()
{
    QStringList namesList;
    QDir dir(ui->pathEdit->text());
    dir.setSorting(QDir::Size);
    dir.setFilter(QDir::Dirs | QDir::Files |QDir::NoDotAndDotDot);
    listFilesRecursive(dir, namesList);
    ui->commentEdit->setPlainText(namesList.join("\n"));
}

void NewEntryDlg::encryptFile()
{
    QSettings settings("ginasoft", "director",this);
    QString lastDir = settings.value("NewEntryDlg/LastDir", qApp->applicationDirPath()).toString();
    QString gpgDir = settings.value("GPG/dir", "/home/visa").toString();
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select file"), lastDir);
    if(filePath.isEmpty())
        return;
    QFileInfo fileInfo(filePath);
    ui->nameEdit->setText(fileInfo.fileName());
    settings.setValue("NewEntryDlg/LastDir", fileInfo.absolutePath());
    ui->pathEdit->setText(filePath);

    // uuid
    QProcess uuidgen;
    uuidgen.start("uuidgen");
    if(!uuidgen.waitForFinished()) {
        qDebug() << "WARNING: " << __FILE__ << ":" << __LINE__;
        return;
    }
    QByteArray uuidBytes = uuidgen.readAll();
    ui->uuidEdit->setText(QString(uuidBytes).trimmed());

    // gpg
    QProcess gpg;
    QStringList arguments;
    arguments << "--trust-model=always";
    arguments << "--yes";
    arguments << "-e";
    arguments << QString("-r%1").arg("D94F8F25");
    arguments << QString("-o%1/%2.gpg").arg(gpgDir).arg(ui->uuidEdit->text());
    arguments << filePath;
    gpg.start("gpg", arguments);
    if(!gpg.waitForFinished(-1)) {
        qDebug() << "WARNING: " << __FILE__ << ":" << __LINE__;
        QMessageBox::warning(this, tr("Warning"), tr("GPG error."));
        return;
    }
    QMessageBox::information(this, tr("Information"), tr("GPG finished."));
}

void NewEntryDlg::encryptDir()
{
    QSettings settings("ginasoft", "director",this);
    QString lastDir = settings.value("NewEntryDlg/LastDir", qApp->applicationDirPath()).toString();
    QString gpgDir = settings.value("GPG/dir", "/home/visa").toString();
    QString dirPath = QFileDialog::getExistingDirectory(this, tr("Select folder"), lastDir);
    if(dirPath.isEmpty())
        return;
    QFileInfo info(dirPath);
    ui->nameEdit->setText(QDir(dirPath).dirName() + QString(".tar.gz"));
    settings.setValue("NewEntryDlg/LastDir", info.absolutePath());
    ui->pathEdit->setText(dirPath);
    // uuid
    QProcess uuidgen;
    uuidgen.start("uuidgen");
    if(!uuidgen.waitForFinished()) {
        qDebug() << "WARNING: " << __FILE__ << ":" << __LINE__;
        return;
    }
    QByteArray uuidBytes = uuidgen.readAll();
    ui->uuidEdit->setText(QString(uuidBytes).trimmed());

    // tar
    QProcess tar;
    QStringList arguments;
    arguments << "-zcf";
    arguments << QString("%1/%2.tar.gz").arg(gpgDir).arg(QDir(dirPath).dirName());
    arguments << QString("-C%1").arg(info.absolutePath());
    arguments << QString("./%1").arg(QDir(dirPath).dirName());
    tar.start("tar", arguments);
    if(!tar.waitForFinished(-1)) {
        qDebug() << "WARNING: " << __FILE__ << ":" << __LINE__;
        QMessageBox::warning(this, tr("Warning"), tr("TAR error."));
        return;
    }
    // encode
    QProcess gpg;
    arguments.clear();
    arguments << "--trust-model=always";
    arguments << "--yes";
    arguments << "-e";
    arguments << QString("-r%1").arg("D94F8F25");
    arguments << QString("-o%1/%2.gpg").arg(gpgDir).arg(ui->uuidEdit->text());
    arguments << QString("%1/%2.tar.gz").arg(gpgDir).arg(QDir(dirPath).dirName());
    //arguments << QString("%1.tar.gz").arg(dirPath);
    gpg.start("gpg", arguments);
    if(!gpg.waitForFinished(-1)) {
        qDebug() << "WARNING: " << __FILE__ << ":" << __LINE__;
        QMessageBox::warning(this, tr("Warning"), tr("GPG error."));
        return;
    }
    QFile::remove(QString("%1/%2.tar.gz").arg(gpgDir).arg(QDir(dirPath).dirName()));
    QMessageBox::information(this, tr("Information"), tr("GPG finished."));
}

void NewEntryDlg::listFilesRecursive(QDir &dir, QStringList &namesList)
{
    QFileInfoList infoList = dir.entryInfoList();
    for(int i = 0; i < infoList.size(); ++i){
        QFileInfo info = infoList.at(i);
        if(info.isFile())
            namesList << info.fileName();
        else if(info.isDir()) {
            dir.cd(info.fileName());
            listFilesRecursive(dir, namesList);
            dir.cdUp();
        }
    }
}
