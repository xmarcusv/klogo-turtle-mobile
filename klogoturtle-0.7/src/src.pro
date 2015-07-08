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
CONFIG += warn_on

DESTDIR = ../bin
MOC_DIR = ../build/moc
RCC_DIR = ../build/rcc
UI_DIR = ../build/ui
unix:OBJECTS_DIR = ../build/o/unix
win32:OBJECTS_DIR = ../build/o/win32
macx:OBJECTS_DIR = ../build/o/mac


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
    klogoturtle.qrc \
    languages.qrc

TRANSLATIONS = languages/klogoturtle_pt_BR.ts  languages/klogoturtle_fr.ts languages/klogoturtle_it.ts languages/klogoturtle_de.ts

