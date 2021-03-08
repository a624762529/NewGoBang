#-------------------------------------------------
#
# Project created by QtCreator 2021-01-19T18:41:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
TARGET = GoBang
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


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    ai_play/aiplaygame.cpp \
    ai_play/record.cpp \
    chessbord/chessboard.cpp \
    socket/sockclient.cpp \
    socket_play/networkgame.cpp \
    two_people_play/twopeoplegame.cpp \
    chessshow.cpp \
    talkwindow.cpp

HEADERS += \
        mainwindow.h \
    ai_play/aiplaygame.h \
    ai_play/record.h \
    chessbord/chessboard.h \
    socket/sockclient.h \
    socket_play/networkgame.h \
    two_people_play/twopeoplegame.h \
    chessshow.h \
    head.h \
    mainwindow.h \
    talkwindow.h

FORMS += \
        mainwindow.ui \
    chessshow.ui \
    talkwindow.ui
