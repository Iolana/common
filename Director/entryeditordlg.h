#ifndef ENTRYEDITORDLG_H
#define ENTRYEDITORDLG_H

#include <QDialog>

namespace Ui {
class EntryEditorDlg;
}

class EntryEditorDlg : public QDialog
{
    Q_OBJECT

public:
    explicit EntryEditorDlg(QWidget *parent = 0);
    ~EntryEditorDlg();
    void setName(const QString& name);
    void setUuid(const QString& uuid);
    void setTags(const QString& tags);
    void setStored(const QString& stored);
    void setComment(const QString& comment);

    QString getName();
    QString getUuid();
    QString getTags();
    QString getStored();
    QString getComment();

private:
    Ui::EntryEditorDlg *ui;
};

#endif // ENTRYEDITORDLG_H
