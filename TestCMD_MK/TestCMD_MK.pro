TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += "xdata=\"\""

DEFINES += "bit=\"unsigned char\""


#TODO DELETE in MK
#//#define xdata
#//typedef uint8_t bit;

win32:INCLUDEPATH += ../T-CARD_MK/Main/
win32:DEPENDPATH += ../T-CARD_MK/Main/

SOURCES += \
        ../T-CARD_MK/Main/CMD_data.c \
        main.c

HEADERS += \
    ../T-CARD_MK/Main/CMD_data.h \
    ../T-CARD_MK/Main/InitType.h


