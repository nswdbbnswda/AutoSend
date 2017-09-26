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
    iter.cpp \
    filebase.cpp \
    filewatcher.cpp \
    sender.cpp \
    mytcpsever.cpp \
    md5key.cpp

HEADERS += \
    server.h \
    client.h \
    iter.h \
    filebase.h \
    filewatcher.h \
    sender.h \
    mytcpsever.h \
    autosend.h \
    md5key.h
