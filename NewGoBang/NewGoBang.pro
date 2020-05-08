#-------------------------------------------------
#
# Project created by QtCreator 2020-05-04T08:39:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NewGoBang
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QT += multimedia

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        chessshow.cpp \
        aiplaygame.cpp \
        chessboard.cpp \
        record.cpp \
        twopeoplegame.cpp \
    networkgame.cpp \
    sockclient.cpp \
    Tube.cpp \
    sem_pv.cpp

HEADERS += \
        mainwindow.h \
        chessshow.h \
        chessboard.h \
        aiplaygame.h \
        record.h \
        twopeoplegame.h \
    networkgame.h \
    sockclient.h \
    head.h \
    server.h \
    sockarry.h \
    retserver.h \
    Tube.h \
    sem_pv.h

FORMS += \
        mainwindow.ui \
    chessshow.ui
CONFIG+=C++11
