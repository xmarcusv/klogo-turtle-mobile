#-------------------------------------------------
#
# Project created by QtCreator 2015-03-31T19:20:55
#
#-------------------------------------------------

QT       += core gui
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = klogoturtle
TEMPLATE = app


SOURCES += main.cpp\
        klogoturtle.cpp \
    desenho.cpp \
    klogoturtlemobileapp.cpp \
    mesamobile.cpp

HEADERS  += klogoturtle.h \
    desenho.h \
    klogoturtlemobileapp.h \
    mesamobile.h

FORMS    += klogoturtle.ui \
    klogoturtlemobileapp.ui \
    mesamobile.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    klogoturtle.qrc

