QT       += widgets

TEMPLATE = lib
DEFINES += STYLEPLUGIN_LIBRARY

CONFIG += c++11 plugin

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    StringHelper.cpp \
    stylemanager.cpp \
    styleplugin.cpp \
    themecolor.cpp \
	StyleEventFilter.cpp

HEADERS += \
    StringHelper.h \
    StylePlugin_global.h \
    stylemanager.h \
    styleplugin.h \
    themecolor.h \
	StyleEventFilter.h

INCLUDEPATH += $$PWD/../include

CONFIG(release, debug|release): {
    DESTDIR = $$PWD/../bin/release
}
else:CONFIG(debug, debug|release): {
    DESTDIR = $$PWD/../bin/debug
}

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qss.qrc

RC_FILE += \
    StylePlugin.rc
