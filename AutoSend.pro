QT += core
QT -= gui
QT += network
CONFIG += c++11

TARGET = AutoSend
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    server.cpp \
    client.cpp \
    iter.cpp

HEADERS += \
    server.h \
    client.h \
    iter.h
