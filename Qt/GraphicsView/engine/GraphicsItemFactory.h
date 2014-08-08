/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */
#ifndef GRAPHICSITEMFACTORY_H
#define GRAPHICSITEMFACTORY_H

#include <QHash>

class QGraphicsItem;
typedef  QGraphicsItem* (*FACTORY_FUNCTION)();

class GraphicsItemFactory {
    QHash<QString, FACTORY_FUNCTION> factoryFunctions;

public:
    GraphicsItemFactory();

    void registerItemClass(const QString& className, FACTORY_FUNCTION fac);

    QGraphicsItem* createItem(const QString& className);
};

#endif
