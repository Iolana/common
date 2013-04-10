TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += \
    XArchDbase \
    TestDbase

TestDbase.depends = XArchDbase
