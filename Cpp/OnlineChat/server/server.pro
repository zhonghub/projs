# qmake -tp vc
QT += core gui

QT += widgets

CONFIG += console qt c++11

DEFINES += QT_DEPRECATED_WARNINGS

TARGET = Server

TEMPLATE = app

SOURCES += z_src/main.cpp \
    z_src/sql.cpp \
    z_src/server.cpp \
    z_src/MyWindow.cpp \
    z_src/ServerController.cpp \
    include\jsoncpp.cpp \

HEADERS +=  z_include/server.h \
    z_include/sql.h \
    z_include/MyWindow.h \
    z_include/ServerController.h \
    include\json\json.h \
    include\json\json-forwards.h \

