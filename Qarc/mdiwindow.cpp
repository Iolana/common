#include "mdiwindow.h"
#include "ui_mdiwindow.h"

MdiWindow::MdiWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MdiWindow)
{
    ui->setupUi(this);
}

MdiWindow::~MdiWindow()
{
    delete ui;
}
