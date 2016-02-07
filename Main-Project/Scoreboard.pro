#-------------------------------------------------
#
# Project created by QtCreator 2015-09-30T17:18:11
#
#-------------------------------------------------

QT       += core gui
QT += network
QT += multimedia
QT += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RC_FILE = myapp.rc

TARGET = Scoreboard
TEMPLATE = app

CONFIG += releas
SOURCES += main.cpp\
        scoreboardmain.cpp \
    dialog.cpp


HEADERS  += scoreboardmain.h \
    dialog.h

FORMS    += scoreboardmain.ui \
    dialog.ui
