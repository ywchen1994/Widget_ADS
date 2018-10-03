#-------------------------------------------------
#
# Project created by QtCreator 2018-10-02T02:47:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Widget_I2C_DSP
TEMPLATE = app
CONFIG += c++11 console
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
        main.cpp \
        mainwindow.cpp \
    ads1115.cpp \
    qcustomplot.cpp

HEADERS += \
        mainwindow.h \
    ads1115.h \
    qcustomplot.h

FORMS += \
        mainwindow.ui

INCLUDEPATH+=/usr/local/include/


LIBS+= /usr/local/lib/libwiringPi.so
LIBS+= /usr/local/lib/libwiringPiDev.so
