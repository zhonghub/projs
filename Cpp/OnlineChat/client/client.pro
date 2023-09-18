# qmake -tp vc
QT += core gui

QT += widgets

CONFIG += console qt c++11

DEFINES += QT_DEPRECATED_WARNINGS

TARGET = client

TEMPLATE = app

SOURCES += ChatWindow.cpp \
    mainwindow.cpp \
    Client.cpp \
    Client2.cpp \
    myUseFun.cpp \
    main.cpp

HEADERS += mainwindow.h \
    chatWindow.h \
    Client.h \
    Client2.h \
    myUseFun.h \


