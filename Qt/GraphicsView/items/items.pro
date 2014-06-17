# This work is licensed under the Creative Commons Attribution 3.0 Unported
# License. To view a copy of this license, visit
# http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
# Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.

TEMPLATE = lib
QT = gui core widgets

CONFIG += qt warn_on debug console

INCLUDEPATH = ../engine
LIBS += -L../engine/debug -lengine

HEADERS = LineItem.h RectItem.h CircleItem.h EllipseItem.h TextItem.h BezierItem.h

SOURCES = StandardItems.cpp LineItem.cpp RectItem.cpp CircleItem.cpp EllipseItem.cpp TextItem.cpp BezierItem.cpp
