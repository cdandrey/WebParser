#-------------------------------------------------
#
# Project created by QtCreator 2019-11-12T23:46:21
#
#-------------------------------------------------

QT += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

TARGET = WebParser
TEMPLATE = app


SOURCES += main.cpp\
    cwidget_control.cpp \
    cwidget_main.cpp \
    cmanager_parsers.cpp \
    cmodel_tasks.cpp \
    cparser.cpp \
    ctasks_pool.cpp

HEADERS  += \
    cwidget_control.h \
    cwidget_main.h \
    cmanager_parsers.h \
    cmodel_tasks.h \
    cparser.h \
    cwebparser_types.h \
    ctasks_pool.h

RESOURCES += \
    ico.qrc \
    qss.qrc

