#ifndef MDIWINDOW_H
#define MDIWINDOW_H

#include <QMainWindow>

namespace Ui {
class MdiWindow;
}

class MdiWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MdiWindow(QWidget *parent = 0);
    ~MdiWindow();
    
private:
    Ui::MdiWindow *ui;
};

#endif // MDIWINDOW_H
