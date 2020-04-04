QT += core gui
QT += serialport
QT += widgets printsupport

TARGET = T-Card
TEMPLATE = app

SOURCES += main.cpp\
    UtilityUWB/errorcodeboost.cpp \
    UtilityUWB/serialport.cpp \
        mainwindow.cpp \
    port.cpp

HEADERS += mainwindow.h \
    UtilityUWB/errorcodeboost.h \
    UtilityUWB/serialport.h \
    port.h

FORMS += mainwindow.ui


win32:LIBS += -LH:/LIBS/boost_1_70_0/stage/lib -llibboost_system-mgw73-mt-x64-1_70
win32:LIBS += -LH:/LIBS/boost_1_70_0/stage/lib -llibboost_thread-mgw73-mt-x64-1_70

win32:INCLUDEPATH += H:/LIBS/boost_1_70_0
win32:DEPENDPATH += H:/LIBS/boost_1_70_0
win32:LIBS += -lsetupapi
win32:LIBS += -lwsock32
win32:LIBS += -lws2_32
win32:DEFINES += _WIN32_WINNT=0x0501
