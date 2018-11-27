#-------------------------------------------------
#
# Project created by QtCreator 2018-11-19T14:40:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = movingRobots
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        robot.cpp \
    source.cpp \
    buffer.cpp \
    vector2d.cpp \
    process.cpp

HEADERS  += mainwindow.h \
    robot.h \
    source.h \
    buffer.h \
    vector2d.h \
    process.h

FORMS    += mainwindow.ui
