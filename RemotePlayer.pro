#-------------------------------------------------
#
# Project created by QtCreator 2013-02-11T22:25:19
#
#-------------------------------------------------

QT       += core gui network phonon

TARGET = RemotePlayer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    httpcontroler.cpp \
    controlthread.cpp \
    controllerrequest.cpp \
    router.cpp

HEADERS  += mainwindow.h \
    httpcontroler.h \
    controlthread.h \
    controllerrequest.h \
    router.h

FORMS    += mainwindow.ui
