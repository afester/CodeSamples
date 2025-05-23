
#-------------------------------------------------
#
# Project created by QtCreator 2019-02-20T12:56:14
#
#-------------------------------------------------

QT       += widgets

TARGET = engine
TEMPLATE = lib

DEFINES += ENGINE_LIBRARY

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
    EditItemInteractor.cpp \
    GraphicsItemFactory.cpp \
    GraphicsScene.cpp \
    GraphicsSheet.cpp \
    Interactor.cpp \
    NewItemInteractor.cpp \
    ScaleEdgeWidget.cpp \
    ScaleWidget.cpp \
    ScrollAreaLayout.cpp \
    Snapper.cpp

HEADERS += \
    EditItemInteractor.h \
    GraphicsItemFactory.h \
    GraphicsScene.h \
    GraphicsSheet.h \
    InteractableItem.h \
    Interactor.h \
    NewItemInteractor.h \
    ScaleEdgeWidget.h \
    ScaleWidget.h \
    ScrollAreaLayout.h \
    Snapper.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
