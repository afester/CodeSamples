#-------------------------------------------------
#
# Project created by QtCreator 2019-02-20T13:02:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sample
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


SOURCES += \
        main.cpp \
    LabelledComboBox.cpp \
    Log.cpp \
    main.cpp \
    objectcontroller.cpp

HEADERS += \
    LabelledComboBox.h \
    Log.h \
    MainWindow.h \
    objectcontroller.h

FORMS +=

RESOURCES += \
    GraphicsView.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../qt-solutions/qtpropertybrowser/lib/ -lQtSolutions_PropertyBrowser-hea
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../qt-solutions/qtpropertybrowser/lib/ -lQtSolutions_PropertyBrowser-head
else:unix: LIBS += -L$$PWD/../../../../qt-solutions/qtpropertybrowser/lib/ -lQtSolutions_PropertyBrowser-hea

INCLUDEPATH += $$PWD/../../../../qt-solutions/qtpropertybrowser/src
DEPENDPATH += $$PWD/../../../../qt-solutions/qtpropertybrowser

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../engine/release/ -lengine
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../engine/debug/ -lengine
else:unix: LIBS += -L$$OUT_PWD/../engine/ -lengine

INCLUDEPATH += $$PWD/../engine
DEPENDPATH += $$PWD/../engine
