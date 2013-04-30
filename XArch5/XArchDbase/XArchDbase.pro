#-------------------------------------------------
#
# Project created by QtCreator 2013-04-10T23:44:20
#
#-------------------------------------------------

QT       += sql

QT       -= gui

TARGET = XArchDbase
TEMPLATE = lib

DEFINES += XARCHDBASE_LIBRARY

SOURCES += dbase.cpp \
    structures.cpp

HEADERS += dbase.h\
        XArchDbase_global.h \
    structures.h

DESTDIR = ../../libs

unix:
{
    target.path = /usr/lib64
    INSTALLS += target
}
