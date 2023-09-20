# qmake -tp vc
QT += core gui

QT += widgets

CONFIG += console qt c++11

DEFINES += QT_DEPRECATED_WARNINGS

TARGET = Server

TEMPLATE = app

SOURCES += main.cpp \
    sql.cpp \
    server.cpp \
    MyWindow.cpp \
    ServerController.cpp \

HEADERS +=  server.h \
    sql.h \
    MyWindow.h \
    ServerController.h \

