QT       += core
QT       += network
QT       -= gui

TARGET = Server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    socketthread.cpp \
    server.cpp

HEADERS += \
    socketthread.h \
    server.h
