/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */
#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>

class QGraphicsItem;
class QXmlStreamReader;
class QXmlStreamWriter;
class GraphicsItemFactory;
typedef  QGraphicsItem* (*FACTORY_FUNCTION)();

#ifdef ENGINE_LIBRARY
class Q_DECL_EXPORT GraphicsScene : public QGraphicsScene {
#else
class Q_DECL_IMPORT GraphicsScene : public QGraphicsScene {
#endif

public:
    GraphicsScene();

    QGraphicsItem* getItemAt(const QPointF & position);

    void readExternal(QXmlStreamReader& reader);

    void writeExternal(QXmlStreamWriter& writer);

    void saveToFile(const QString& fileName);

    void loadFromFile(const QString& fileName);

    QList<QString> getSupportedItemClasses();

    FACTORY_FUNCTION getFactoryFunction(const QString& itemClass);

private:
    GraphicsItemFactory* itemFactory;
};

#endif
