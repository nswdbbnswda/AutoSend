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
    filewatcher.cpp \
    sender.cpp \
    mytcpsever.cpp \
    md5key.cpp \
    pathremake.cpp \
    factoary.cpp

HEADERS += \
    server.h \
    client.h \
    filewatcher.h \
    sender.h \
    mytcpsever.h \
    autosend.h \
    md5key.h \
    pathremake.h \
    Factoary.h
