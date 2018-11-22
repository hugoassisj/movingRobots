#-------------------------------------------------
#
# Project created by QtCreator 2018-11-19T14:40:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = projetoSO
TEMPLATE = app
CONFIG += console


SOURCES += main.cpp\
        mainwindow.cpp \
        robot.cpp \
    source.cpp \
    buffer.cpp \
    vector2d.cpp

HEADERS  += mainwindow.h \
    robot.h \
    source.h \
    buffer.h \
    vector2d.h

FORMS    += mainwindow.ui
