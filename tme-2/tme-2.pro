#-------------------------------------------------
#
# Project created by QtCreator 2014-11-28T00:13:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++11
LIBS += -L/home/john/Documents/cpp/qt2/SFML/build/lib -lsfml-graphics -lsfml-window -lsfml-system
LIBS += -L/usr/include/X11 -lX11

TARGET = tme-1
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    qsfmlcanvas.cpp \
    mycanvas.cpp \
    jradiobutton.cpp \
    jpushbutton.cpp \
    line.cpp \
    tile.cpp \
    tilesheet.cpp \
    tilesheethandler.cpp \
    newmapdialog.cpp \
    tilemap.cpp \
    utility.cpp \
    jsoncpp.cpp

HEADERS  += mainwindow.h \
    qsfmlcanvas.h \
    mycanvas.h \
    jradiobutton.h \
    utility.h \
    jpushbutton.h \
    line.h \
    tile.h \
    tilesheet.h \
    tilesheethandler.h \
    newmapdialog.h \
    tilemap.h \
    json/json.h \
    json/json-forwards.h

FORMS    += \
    mainwindow.ui

RESOURCES += \
    tiles.qrc
