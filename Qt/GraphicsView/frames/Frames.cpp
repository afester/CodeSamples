#include <QDebug>

#include "GraphicsItemFactory.h"
#include "DefaultFrame.h"

extern "C" {

void Q_DECL_EXPORT ItemsMain(GraphicsItemFactory* itemFactory) {
   itemFactory->registerItemClass("DefaultFrame", DefaultFrame::create);
}

}
