#ifndef STRUCTURES_H
#define STRUCTURES_H

#endif // STRUCTURES_H

#include <QString>
#include <QDate>

class Patient
{
public:
    enum Sex
    {
        SEX_UNDEFINED = -1,
        SEX_MALE = 0,
        SEX_FEMALE = 1
    };
    Patient();
    int getId();
    QString getName();
    QDate getBirthday();
    Sex getSex();
    int getExaminationsCount();
    QList<int> getExaminationsIds();
};
