#include <QDebug>

#include "GraphicsItemFactory.h"
#include "DefaultFrame.h"

extern "C" {

void ItemsMain(GraphicsItemFactory* itemFactory) {
   itemFactory->registerItemClass("DefaultFrame", DefaultFrame::create);
}

}
