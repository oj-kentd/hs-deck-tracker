#-------------------------------------------------
#
# Project created by QtCreator 2014-04-27T16:50:17
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hs-deck-tracker
TEMPLATE = app

VERSION = 0.3
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += QT_USE_QSTRINGBUILDER
CONFIG(debug, debug|release):CONFIG += console

SOURCES += main.cpp\
        mainwindow.cpp \
    activedeck.cpp \
    cardwidget.cpp \
    newdeckdialog.cpp \
    deckwidget.cpp \
    carddata.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    activedeck.h \
    cardwidget.h \
    newdeckdialog.h \
    deckwidget.h \
    HearthstoneApi.h \
    carddata.h \
    aboutdialog.h

FORMS    += mainwindow.ui \
    activedeck.ui \
    cardwidget.ui \
    newdeckdialog.ui \
    deckwidget.ui \
    aboutdialog.ui

RESOURCES += \
    images.qrc

OTHER_FILES += \
    README.md
