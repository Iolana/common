#-------------------------------------------------
#
# Project created by QtCreator 2014-02-16T12:11:22
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Director
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dbworker.cpp \
    entryeditordlg.cpp \
    newentrydlg.cpp

HEADERS  += mainwindow.h \
    dbworker.h \
    entryeditordlg.h \
    newentrydlg.h

FORMS    += mainwindow.ui \
    entryeditordlg.ui \
    newentrydlg.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    folder.png \
    file.png \
    opt.png \
    remove.png
