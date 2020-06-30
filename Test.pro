#-------------------------------------------------
#
# Project created by QtCreator 2019-04-18T11:44:19
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
QT       += sql
#QT       += hidapi
#QT       += charts
QT += charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RC_FILE += exe.rc

TARGET = Test
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11
CONFIG += resources_big
SOURCES += \
        about.cpp \
        averagesubframe.cpp \
        comdata.cpp \
        debugwatch.cpp \
        historydetail.cpp \
        historyview.cpp \
        main.cpp \
        mainwindow.cpp \
        qchartviewer.cpp \
        realtime.cpp \
        sqlite_thread.cpp \
        usb_hid.cpp \
        usb_receive_thread.cpp \
        usb_send_thread.cpp

HEADERS += \
        about.h \
        about.h \
        averagesubframe.h \
        comdata.h \
        debugwatch.h \
        historydetail.h \
        historyview.h \
        mainwindow.h \
        qchartviewer.h \
        realtime.h \
        sqlite_thread.h \
        usb_hid.h \
        hidapi.h \
        libusb.h \
        usb_receive_thread.h \
        usb_send_thread.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# unix|win32: LIBS += -lQt5SerialPort

DISTFILES += \
    exe.rc

RESOURCES += \
    Resource/realtimeres.qrc

win32:CONFIG(release, debug|release):LIBS += -L$$PWD/lib32/ -lchartdir60
else:win32:CONFIG(debug, debug|release):LIBS += -L$$PWD/lib32/ -lchartdir60
else:unix: LIBS += -L$$PWD/lib32/ -lchartdir60

INCLUDEPATH += $$PWD/lib32
DEPENDPATH += $$PWD/lib32

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib32/ -lhidapi
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib32/ -lhidapi
#else:unix: LIBS += -L$$PWD/lib32/ -lhidapi

INCLUDEPATH += $$PWD/lib32
DEPENDPATH += $$PWD/lib32

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib32/ -llibusb-1.0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib32/ -llibusb-1.0
else:unix: LIBS += -L$$PWD/lib32/ -llibusb-1.0

INCLUDEPATH += $$PWD/lib32
DEPENDPATH += $$PWD/lib32
