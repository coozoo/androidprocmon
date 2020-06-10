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
CONFIG += lrelease

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


win32:RC_FILE = myapp.rc
macx:RC_FILE = computer.icns

RESOURCES += \
    resources.qrc

DISTFILES +=

binary.files += $$TARGET
binary.path = /usr/bin
translations.files += $$files(.qm/*.qm,true)
translations.path = /usr/share/$$TARGET
icon.files +=iamges/computer.png
icon.path += /usr/share/icons
desktop.files += $$TARGET.desktop
desktop.path += /usr/share/applications/
INSTALLS += binary translations icon desktop
