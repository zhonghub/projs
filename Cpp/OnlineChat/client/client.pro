# qmake -tp vc
QT += core gui

QT += widgets

CONFIG += console qt c++11

DEFINES += QT_DEPRECATED_WARNINGS

TARGET = client

TEMPLATE = app

SOURCES += z_src/ChatWindow.cpp \
    z_src/MainWindow.cpp \
    z_src/Client.cpp \
    z_src/ClientController.cpp \
    z_src/myUseFun.cpp \
    z_src/main.cpp

HEADERS += z_include/MainWindow.h \
    z_include/ChatWindow.h \
    z_include/Client.h \
    z_include/ClientController.h \
    z_include/myUseFun.h \


