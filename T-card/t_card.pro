QT += core gui
QT += serialport
QT += widgets printsupport

TARGET = T-Card
TEMPLATE = app

SOURCES += main.cpp\
    CMD_data.cpp \
    UtilityUWB/errorcodeboost.cpp \
    UtilityUWB/serialport.cpp \
        mainwindow.cpp \
    port.cpp \
    rfid/MFRC522.cpp \
    rfid/ReadAndWrite.cpp \
    rfid/callback.cpp \
    rfid/mfrc.cpp

HEADERS += mainwindow.h \
    CMD_data.h \
    UtilityUWB/errorcodeboost.h \
    UtilityUWB/serialport.h \
    port.h \
    rfid/MFRC522.h \
    rfid/ReadAndWrite.h \
    rfid/callback.h \
    rfid/deprecated.h \
    rfid/mfrc.h \
    rfid/require_cpp11.h

FORMS += mainwindow.ui


win32:LIBS += -LH:/LIBS/boost_1_70_0/stage/lib -llibboost_system-mgw73-mt-x64-1_70
win32:LIBS += -LH:/LIBS/boost_1_70_0/stage/lib -llibboost_thread-mgw73-mt-x64-1_70

win32:INCLUDEPATH += H:/LIBS/boost_1_70_0
win32:DEPENDPATH += H:/LIBS/boost_1_70_0
win32:LIBS += -lsetupapi
win32:LIBS += -lwsock32
win32:LIBS += -lws2_32
win32:DEFINES += _WIN32_WINNT=0x0501
