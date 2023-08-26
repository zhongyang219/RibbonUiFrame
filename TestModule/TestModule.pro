#-------------------------------------------------
#
# Project created by QtCreator 2021-03-24T17:35:25
#
#-------------------------------------------------

QT       += widgets

CONFIG += c++11

TARGET = TestModule
TEMPLATE = lib

DEFINES += TESTMODULE_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        mainwidget.cpp \
        testmodule.cpp

HEADERS += \
        mainwidget.h \
        testmodule.h \
        testmodule_global.h  

INCLUDEPATH += $$PWD/../include

CONFIG(release, debug|release): {
    DESTDIR = $$PWD/../bin/release
}
else:CONFIG(debug, debug|release): {
    DESTDIR = $$PWD/../bin/debug
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    mainwidget.ui

RESOURCES += \
    resource.qrc
