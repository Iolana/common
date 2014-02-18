#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

namespace Ui {
class MainWindow;
}
class QStringListModel;
class DbWorker;
class QAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_searchBtn_clicked();
    void on_listView_doubleClicked(const QModelIndex& index);
    void listView_selectionChanged(QModelIndex index);
    //**
    void addFile();
    void addFolder();
    void options();

private:
    void setupActions();
    void setupMenus();
    void setupToolbars();

private:
    Ui::MainWindow *ui;
    QStringListModel* mainModel;
    DbWorker* db;

    QAction* addFileAct;
    QAction* addFolderAct;
    QAction* exitAct;
    QAction* optionsAct;
};

#endif // MAINWINDOW_H
