#-------------------------------------------------
# Project created by QtCreator 2024-03-09T12:00:00
#-------------------------------------------------

QT       += core gui widgets

CONFIG += c++17

# Uncomment this line to disable deprecated APIs before Qt 6.0.0
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

# Define your sources and headers
SOURCES += \
    main.cpp \
    mainWindow/mainwindow.cpp \
    Buffer/buffer.cpp \
    Processor/processor.cpp \
    Robot/robot.cpp \
    Source/source.cpp \
    Vector2D/vector2d.cpp

HEADERS += \
    mainWindow/mainwindow.h \
    Buffer/buffer.h \
    Processor/processor.h \
    Robot/robot.h \
    Source/source.h \
    Vector2D/vector2d.h

FORMS += \
    mainWindow/mainwindow.ui

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
