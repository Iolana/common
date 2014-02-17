#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

namespace Ui {
class MainWindow;
}
class QStringListModel;
class DbWorker;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_searchBtn_clicked();
    void listView_clicked(QModelIndex index);

private:
    Ui::MainWindow *ui;
    QStringListModel* mainModel;
    DbWorker* db;
};

#endif // MAINWINDOW_H
