#-------------------------------------------------
#
# Project created by QtCreator 2016-10-07T10:31:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = androidprocmon
TRANSLATIONS = androidprocmon_en.ts androidprocmon_uk_UA.ts
CODECFORSRC = UTF-8
TEMPLATE = app

CONFIG += c++11


SOURCES += main.cpp\
        mainwindow.cpp \
    adbviewer.cpp \
    qplaintexteditcursor.cpp \
    qcustomplot.cpp \
    chartmanager.cpp \
    chartrealtime.cpp \
    maincontainer.cpp \
    adbexecute.cpp

HEADERS  += mainwindow.h \
    adbviewer.h \
    qplaintexteditcursor.h \
    qcustomplot.h \
    chartmanager.h \
    chartrealtime.h \
    maincontainer.h \
    maincontainerform.h \
    adbexecute.h

FORMS    += mainwindow.ui


RC_FILE = myapp.rc

RESOURCES += \
    resources.qrc

DISTFILES +=
