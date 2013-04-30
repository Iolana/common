#include "structures.h"

Patient::Patient()
{
}

int Patient::getId()
{
    return -1;
}

QString Patient::getName()
{
    return QString();
}

QDate Patient::getBirthday()
{
    return QDate();
}

Patient::Sex Patient::getSex()
{
    return SEX_UNDEFINED;
}

int Patient::getExaminationsCount()
{
    return 0;
}

QList<int> Patient::getExaminationsIds()
{
    QList<int> list;
    return list;
}

