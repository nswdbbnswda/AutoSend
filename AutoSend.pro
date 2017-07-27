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
    sendthread.cpp \
    filebase.cpp \
    filewatcher.cpp \
    sender.cpp \
    mytcpsever.cpp \
    mytcpsocket.cpp

HEADERS += \
    server.h \
    client.h \
    iter.h \
    sendthread.h \
    filebase.h \
    filewatcher.h \
    sender.h \
    mytcpsever.h \
    mytcpsocket.h
