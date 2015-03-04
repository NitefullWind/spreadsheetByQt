#-------------------------------------------------
#
# Project created by QtCreator 2015-03-01T21:15:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Spreadsheet
TEMPLATE = app


SOURCES += main.cpp\
    finddialog.cpp \
    gotocelldialog.cpp \
    sortdialog.cpp \
    mainwindow.cpp

HEADERS  += \
    finddialog.h \
    gotocelldialog.h \
    sortdialog.h \
    mainwindow.h

FORMS    += mainwindow.ui \
    gotocelldialog.ui \
    sortdialog.ui

RESOURCES += \
    spreadsheet.qrc
