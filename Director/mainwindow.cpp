#include <QDebug>
#include <QStringListModel>
#include <QSettings>
#include <QMessageBox>
#include <QToolBar>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dbworker.h"
#include "entryeditordlg.h"
#include "newentrydlg.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Director of private data");
    QSettings settings("ginasoft", "director");
    resize(settings.value("Main/size", QSize(800, 600)).toSize());
    move(settings.value("Main/pos", QPoint(50, 50)).toPoint());
    setupActions();
    setupMenus();
    setupToolbars();

    db = new DbWorker("/home/visa/Dropbox/mydb.db", this);
    mainModel = new QStringListModel(this);
    ui->listView->setModel(mainModel);
    connect(ui->listView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(listView_selectionChanged(QModelIndex)));

    statusBar()->showMessage(tr("Ready"), 2000);
}

MainWindow::~MainWindow()
{
    QSettings settings("ginasoft", "director");
    settings.setValue("Main/size", size());
    settings.setValue("Main/pos", pos());
    delete ui;
}

void MainWindow::on_searchBtn_clicked()
{
    QString name = ui->nameEdit->text();
    QString tag = ui->tagEdit->text();
    QStringList list = db->find(name, tag);
    mainModel->setStringList(list);
    ui->listView->setModel(mainModel);
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    QString oldName = index.data().toString();
    EntryEditorDlg dlg;
    dlg.setName(oldName);
    dlg.setUuid(ui->uuidLabel->text());
    dlg.setTags(ui->tagsLabel->text());
    dlg.setStored(ui->storedLabel->text());
    dlg.setComment(ui->commentText->toPlainText());
    if(dlg.exec() == QDialog::Accepted) {
        QString name = dlg.getName();
        QString uuid = dlg.getUuid();
        QString tags = dlg.getTags();
        QString stored = dlg.getStored();
        QString comment = dlg.getComment();
        if(db->update(oldName, name, uuid, tags, stored, comment)) {
            on_searchBtn_clicked();
        }
        else
            QMessageBox::critical(this, tr("Error"), tr("Unable to update"));
    }
}

void MainWindow::listView_selectionChanged(QModelIndex index)
{
    QString uuid, tags, stored, comment;
    db->getData(index.data().toString(), uuid, tags, stored, comment);
    ui->uuidLabel->setText(uuid);
    ui->tagsLabel->setText(tags);
    ui->storedLabel->setText(stored);
    ui->commentText->setText(comment);
}

void MainWindow::addFile()
{
    NewEntryDlg dlg;
    dlg.setFileMode(true);
    if(dlg.exec() == QDialog::Accepted) {
        QString name = dlg.getName();
        QString uuid = dlg.getUuid();
        QString tags = dlg.getTags();
        QString stored = dlg.getStored();
        QString comment = dlg.getComment();
        if(db->insert(name, uuid, tags, stored, comment)) {
            on_searchBtn_clicked();
        }
        else
            QMessageBox::critical(this, tr("Error"), tr("Unable to insert"));
    }
}

void MainWindow::addFolder()
{
    NewEntryDlg dlg;
    dlg.setFileMode(false);
    if(dlg.exec() == QDialog::Accepted) {
        QString name = dlg.getName();
        QString uuid = dlg.getUuid();
        QString tags = dlg.getTags();
        QString stored = dlg.getStored();
        QString comment = dlg.getComment();
        if(db->insert(name, uuid, tags, stored, comment)) {
            on_searchBtn_clicked();
        }
        else
            QMessageBox::critical(this, tr("Error"), tr("Unable to insert"));
    }
}

void MainWindow::options()
{

}

void MainWindow::remove()
{
    QModelIndex index = ui->listView->currentIndex();
    if(index.isValid())
        if(!db->remove(index.data().toString())) {
            QMessageBox::critical(this, tr("Error"), tr("Unable to remove"));
            return;
        }
    on_searchBtn_clicked();
}

void MainWindow::extract()
{
    QModelIndex index = ui->listView->currentIndex();
    if(!index.isValid())
        return;
    QString name = index.data().toString();
    QString uuid;
    db->getUuid(name, uuid);

    QSettings settings("ginasoft", "director");
    QString extractFromDir = settings.value("Main/extractFromDir",
                         QDir::homePath()).toString();
    QString extractToDir = settings.value("Main/extractToDir",
                         QDir::homePath()).toString();
    QString gpgName = QString("%1.gpg").arg(uuid);
    QString sourceFile = QFileDialog::getOpenFileName(this, tr("Select file"),
                         extractFromDir, gpgName);
    if(sourceFile.isEmpty())
        return;
    settings.setValue("Main/extractFromDir", QFileInfo(sourceFile).absolutePath());

    QString targetDir = QFileDialog::getExistingDirectory(this, tr("Decrypt to"),
                         extractToDir);
    if(targetDir.isEmpty())
        return;
    settings.setValue("Main/extractToDir", targetDir);

    QFile::link(sourceFile, QString("%1/%2.gpg").arg(extractToDir).arg(name));

}

void MainWindow::setupActions()
{
    addFileAct = new QAction(QIcon(":/images/file.png"), tr("Add file..."), this);
    addFileAct->setShortcut(tr("Ctrl+N"));
    addFileAct->setStatusTip(tr("Add new file to collection."));
    connect(addFileAct, SIGNAL(triggered()), this, SLOT(addFile()));

    addFolderAct = new QAction(QIcon(":/images/folder.png"), tr("Add folder..."), this);
    addFolderAct->setShortcut(tr("Ctrl+M"));
    addFolderAct->setStatusTip(tr("Add new folder to collection."));
    connect(addFolderAct, SIGNAL(triggered()), this, SLOT(addFolder()));

    removeAct = new QAction(QIcon(":/images/remove.png"), tr("Remove entry"), this);
    removeAct->setStatusTip(tr("Removes entry from collection."));
    connect(removeAct, SIGNAL(triggered()), this, SLOT(remove()));

    extractAct = new QAction(QIcon(":/images/extract.png"), tr("Extract entry..."), this);
    extractAct->setShortcut(tr("Ctrl+E"));
    extractAct->setStatusTip(tr("Decrypts given entry."));
    connect(extractAct, SIGNAL(triggered()), this, SLOT(extract()));

    exitAct = new QAction(tr("Exit"), this);
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(quit()));

    optionsAct = new QAction(QIcon(":/images/opt.png"), tr("Options..."), this);
    connect(optionsAct, SIGNAL(triggered()), this, SLOT(options()));
}

void MainWindow::setupMenus()
{
    QMenu* fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(addFileAct);
    fileMenu->addAction(addFolderAct);
    fileMenu->addAction(extractAct);
    fileMenu->addAction(removeAct);
    fileMenu->addAction(optionsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
}

void MainWindow::setupToolbars()
{
    QToolBar* fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(addFileAct);
    fileToolBar->addAction(addFolderAct);
    fileToolBar->addAction(extractAct);
    fileToolBar->addAction(removeAct);
    fileToolBar->addAction(optionsAct);
}
