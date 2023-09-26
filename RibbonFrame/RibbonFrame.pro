#-------------------------------------------------
#
# Project created by QtCreator 2020-10-28T09:10:15
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = RibbonFrame
TEMPLATE = lib

DEFINES += RIBBONFRAME_LIBRARY


SOURCES += \
    ribbonframewindow.cpp \
    settingsdialog.cpp

HEADERS  += \
    ../include/mainframe_global.h \
    ../include/mainframeinterface.h \
    ../include/moduleinterface.h \
    ../include/ribbonframewindow.h \
    ../include/ribbonuipredefine.h \
    settingsdialog.h

CONFIG(release, debug|release): {
    DESTDIR = $$PWD/../bin/release
}
else:CONFIG(debug, debug|release): {
    DESTDIR = $$PWD/../bin/debug
}

INCLUDEPATH += $$PWD/../include

RESOURCES += \
    ribbonframe.qrc

RC_FILE += \
    RibbonFrame.rc

#关闭“C4100：未引用的形参”警告
win32-msvc* {
    QMAKE_CXXFLAGS *=  /wd"4100"
    contains (QMAKE_CXXFLAGS_WARN_ON, -w34100) : QMAKE_CXXFLAGS_WARN_ON -= -w34100
}

FORMS += \
    settingsdialog.ui
