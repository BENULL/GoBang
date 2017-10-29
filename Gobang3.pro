#-------------------------------------------------
#
# Project created by QtCreator 2016-10-03T15:24:41
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Gobang3
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    GameWindow.cpp \
    Game.cpp \
    NetGameWindow.cpp \
    PeopleVsComputer.cpp

HEADERS  += MainWindow.h \
    GameWindow.h \
    Game.h \
    NetGameWindow.h \
    PeopleVsComputer.h

FORMS    += MainWindow.ui \
    GameWindow.ui \
    NetGameWindow.ui \
    PeopleVsComputer.ui

RESOURCES += \
    resource.qrc
