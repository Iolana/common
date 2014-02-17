#include <QDebug>
#include <QStringListModel>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dbworker.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->listView, SIGNAL(clicked(QModelIndex)), this, SLOT(listView_clicked(QModelIndex)));

    db = new DbWorker("/home/visa/Dropbox/mydb.db", this);
    mainModel = new QStringListModel(this);
}

MainWindow::~MainWindow()
{
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
    qDebug() << index.data().toString();
}
