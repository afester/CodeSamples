#-------------------------------------------------
#
# Project created by QtCreator 2019-02-20T12:58:38
#
#-------------------------------------------------

QT       += widgets

TARGET = items
TEMPLATE = lib

DEFINES += ITEMS_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BezierItem.cpp \
    CircleItem.cpp \
    EllipseItem.cpp \
    LineItem.cpp \
    RectItem.cpp \
    StandardItems.cpp \
    TextItem.cpp

HEADERS += \
    BezierItem.h \
    CircleItem.h \
    EllipseItem.h \
    LineItem.h \
    RectItem.h \
    TextItem.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../engine/release/ -lengine
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../engine/debug/ -lengine
else:unix: LIBS += -L$$OUT_PWD/../engine/ -lengine

DESTDIR = ..\sample\debug

INCLUDEPATH += $$PWD/../engine
DEPENDPATH += $$PWD/../engine
