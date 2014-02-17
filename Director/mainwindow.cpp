#include <QDebug>
#include <QStringListModel>
#include <QSettings>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dbworker.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Director of private data");
    QSettings settings("ginasoft", "director");
    resize(settings.value("Main/size", QSize(800, 600)).toSize());
    move(settings.value("Main/pos", QPoint(50, 50)).toPoint());

    db = new DbWorker("/home/visa/Dropbox/mydb.db", this);
    mainModel = new QStringListModel(this);
    ui->listView->setModel(mainModel);
    connect(ui->listView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(listView_clicked(QModelIndex)));
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

void MainWindow::listView_clicked(QModelIndex index)
{
    QString uuid, tags, stored, comment;
    db->getData(index.data().toString(), uuid, tags, stored, comment);
    ui->uuidLabel->setText(uuid);
    ui->tagsLabel->setText(tags);
    ui->storedLabel->setText(stored);
    ui->commentText->setText(comment);
}
