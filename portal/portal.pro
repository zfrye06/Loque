#-------------------------------------------------
#
# Project created by QtCreator 2016-11-21T12:46:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = portal
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

unix:!macx {
LIBS += -L/usr/local/lib -lsfml-network -lsfml-system
INCLUDEPATH += /ur/local/include/SFML
}
macx:  {
LIBS += -L/usr/local/Cellar/sfml/2.3.2/lib -lsfml-network -lsfml-system
INCLUDEPATH += /usr/local/Cellar/sfml/2.3.2/include
}
