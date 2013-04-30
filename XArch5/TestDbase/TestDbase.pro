#-------------------------------------------------
#
# Project created by QtCreator 2013-04-10T23:45:05
#
#-------------------------------------------------

QT       += testlib sql

QT       -= gui

TARGET = tst_testdbase
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../XArchDbase
unix:
{
    LIBS += ../../libs/libXArchDbase.so
}

SOURCES += tst_testdbase.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
