#include <QDebug>

#include "GraphicsSheet.h"
#include "RectItem.h"
#include "LineItem.h"
#include "EllipseItem.h"
#include "CircleItem.h"
#include "TextItem.h"
#include "BezierItem.h"

extern "C" {

void ItemsMain(GraphicsItemFactory* itemFactory) {
   qDebug() << "In ItemsMain()";

   itemFactory->registerItemClass("RectItem", RectItem::create);
   itemFactory->registerItemClass("LineItem", LineItem::create);
   itemFactory->registerItemClass("EllipseItem", EllipseItem::create);
   itemFactory->registerItemClass("CircleItem", CircleItem::create);
   itemFactory->registerItemClass("TextItem", TextItem::create);
   itemFactory->registerItemClass("BezierItem", BezierItem::create);
}

}
