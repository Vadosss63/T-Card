QT -= core gui

TARGET = UtilityUWB
TEMPLATE = lib
VERSION = 1.0.0.0
CONFIG += shared
CONFIG += dll

SOURCES += \
    networkuwb.cpp \
    ptreewr.cpp \
    timework.cpp \
    serialport.cpp

HEADERS += \
    networkuwb.h \
    ptreewr.h \
    timer.h \
    timework.h \
    serialport.h

headers.files = $$HEADERS

target.path = ../UtilityUWB_so
headers.path= ../UtilityUWB_so/include/
INSTALLS += target headers
