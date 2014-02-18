#include <QDebug>
#include <QStringListModel>
#include <QSettings>
#include <QMessageBox>
#include <QToolBar>
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
        return;
    }
}

void MainWindow::addFolder()
{
    NewEntryDlg dlg;
    dlg.setFileMode(false);
    if(dlg.exec() == QDialog::Accepted) {
        return;
    }
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

    exitAct = new QAction(tr("Exit"), this);
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::setupMenus()
{
    QMenu* fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(addFileAct);
    fileMenu->addAction(addFolderAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
}

void MainWindow::setupToolbars()
{
    QToolBar* fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(addFileAct);
    fileToolBar->addAction(addFolderAct);
}
