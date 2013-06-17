#-------------------------------------------------
#
# Project created by QtCreator 2013-06-17T22:40:46
#
#-------------------------------------------------

QT       += core gui testlib sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qarc
TEMPLATE = app


SOURCES += main.cpp\
        mdiwindow.cpp \
    tst_testdbase.cpp \
    dbase.cpp \
    structures.cpp

HEADERS  += mdiwindow.h \
    dbase.h \
    structures.h \
    tst_testdbase.h

FORMS    += mdiwindow.ui
