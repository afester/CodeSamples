# This work is licensed under the Creative Commons Attribution 3.0 Unported
# License. To view a copy of this license, visit
# http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
# Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.

TEMPLATE = lib
QT = gui core widgets

CONFIG += qt warn_on debug console

DESTDIR = ../bin

HEADERS = ScrollAreaLayout.h ScaleWidget.h ScaleEdgeWidget.h GraphicsSheet.h GraphicsScene.h GraphicsItemFactory.h InteractableItem.h \
          Interactor.h EditItemInteractor.h NewItemInteractor.h \
          Snapper.h

SOURCES = ScrollAreaLayout.cpp ScaleWidget.cpp ScaleEdgeWidget.cpp GraphicsSheet.cpp GraphicsScene.cpp GraphicsItemFactory.cpp \
          Interactor.cpp EditItemInteractor.cpp NewItemInteractor.cpp \
          Snapper.cpp
