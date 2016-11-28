#-------------------------------------------------
#
# Project created by QtCreator 2016-11-23T11:16:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = portal
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    loginwindow.cpp \
    ../shared/loqueclient.cpp \
    adminwindow.cpp

HEADERS  += mainwindow.h \
    loginwindow.h \
    ../shared/loqueclient.h \
    adminwindow.h


FORMS    += mainwindow.ui \
    loginwindow.ui \
    adminwindow.ui

unix:!macx {
LIBS += -L/usr/local/lib -lsfml-network -lsfml-system
INCLUDEPATH += /ur/local/include/SFML
}
macx:  {
LIBS += -L/usr/local/Cellar/sfml/2.3.2/lib -lsfml-network -lsfml-system
INCLUDEPATH += /usr/local/Cellar/sfml/2.3.2/include
}
