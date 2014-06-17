/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */


#include "GraphicsSheet.h"
#include "GraphicsItemFactory.h"

GraphicsItemFactory::GraphicsItemFactory() {
}


void GraphicsItemFactory::registerItemClass(const QString& className, FACTORY_FUNCTION fac) {
    factoryFunctions.insert(className, fac);
}


QGraphicsItem* GraphicsItemFactory::createItem(const QString& className) {
    QGraphicsItem* result = 0;

    FACTORY_FUNCTION fac = factoryFunctions.value(className);
    if (fac) {
        result = fac();
    }

    return result;
}
