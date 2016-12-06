#-------------------------------------------------
#
# Project created by QtCreator 2016-11-23T11:16:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = portal
TEMPLATE = app


SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    ../shared/loqueclient.cpp \
    src/loginpane.cpp \
    src/registerpane.cpp \
    src/studentplaypane.cpp \
    src/adminplaypane.cpp \
    src/adminpane.cpp \
    src/classtab.cpp

HEADERS  += include/mainwindow.h \
    ../shared/loqueclient.h \
    include/loginpane.h \
    include/registerpane.h \
    include/studentplaypane.h \
    include/adminplaypane.h \
    include/adminpane.h \
    include/classtab.h


FORMS    += src/mainwindow.ui \
    src/loginpane.ui \
    src/registerpane.ui \
    src/adminplaypane.ui \
    src/adminpane.ui \
    src/classtab.ui

unix:!macx {
LIBS += -L/usr/local/lib -lsfml-network -lsfml-system
INCLUDEPATH += /usr/local/include/SFML
INCLUDEPATH += /usr/include/SFML
INCLUDEPATH += ./include
}
macx:  {
LIBS += -L/usr/local/Cellar/sfml/2.3.2/lib -L/usr/local/Cellar/sfml/2.4.0/lib -lsfml-network -lsfml-system
INCLUDEPATH += /usr/local/Cellar/sfml/2.3.2/include /usr/local/Cellar/sfml/2.4.0/include
INCLUDEPATH += ./include
}

RESOURCES += \
    resources.qrc
