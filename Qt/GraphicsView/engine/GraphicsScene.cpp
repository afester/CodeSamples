/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */


#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QFile>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include "InteractableItem.h"
#include "GraphicsScene.h"
#include "GraphicsItemFactory.h"


GraphicsScene::GraphicsScene() : QGraphicsScene()  {
    itemFactory = new GraphicsItemFactory();
}


QGraphicsItem* GraphicsScene::getItemAt(const QPointF & position) {
    QGraphicsItem* itm = QGraphicsScene::itemAt ( position, QTransform());
    if (itm) {
        // special case for the parent/child relationship of text frames
        if (itm->parentItem()) {
            itm = itm->parentItem();
        }
    }

    return itm;
}


void GraphicsScene::writeExternal(QXmlStreamWriter& writer) {
    writer.writeStartDocument();

    writer.writeStartElement(metaObject()->className());
    writer.writeAttribute("width", QString::number(width()));
    writer.writeAttribute("height", QString::number(height()));
    writer.writeAttribute("bgColor", backgroundBrush().color().name());

    QList<QGraphicsItem*> allItems = items(Qt::AscendingOrder);
    QGraphicsItem* item;
    foreach(item, allItems) {
        if (item->parentItem() == 0) {
            InteractableItem* writableItem = dynamic_cast<InteractableItem*>(item);
            if (writableItem) {
                writableItem->writeExternal(writer);
            }
        }
    }

    writer.writeEndElement();
    writer.writeEndDocument();
}


void GraphicsScene::saveToFile(const QString& fileName) {
    // Create output XML stream
    QFile destFile(fileName);
    destFile.open(QIODevice::WriteOnly | QIODevice::Text);

    QXmlStreamWriter writer(&destFile);
    writer.setAutoFormatting(true);

    writeExternal(writer);

    destFile.close();

//    undoStack->clear();
}


void GraphicsScene::readExternal(QXmlStreamReader& reader) {
//    undoStack->clear();
    clear();

//    zOrder = 0;

    while(!reader.atEnd()) {
        QXmlStreamReader::TokenType type = reader.readNext();
        if (type == QXmlStreamReader::StartElement) {
            QString elementName = reader.name().toString();
            if (elementName == metaObject()->className()) {
                QString width = reader.attributes().value("width").toString();
                QString height = reader.attributes().value("height").toString();
                QString bgColor = reader.attributes().value("bgColor").toString();
                QString borderWidth = reader.attributes().value("borderWidth").toString();

                setSceneRect(QRectF(0, 0, width.toInt(), height.toInt()));
                setBackgroundBrush(QColor(bgColor));
            } else {
                QGraphicsItem* item = itemFactory->createItem(elementName);
                InteractableItem* readableItem = dynamic_cast<InteractableItem*>(item);
                if (readableItem) {
                    readableItem->readExternal(reader);
                    // item->setZValue(zOrder++);
                    addItem(item);
                }
            }
        }
    }
}


void GraphicsScene::loadFromFile(const QString& fileName) {
    // load the given file into the scene
    QFile srcFile(fileName);
    srcFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QXmlStreamReader stream(&srcFile);
    readExternal(stream);
    srcFile.close();
}


QList<QString> GraphicsScene::getSupportedItemClasses() {
    return itemFactory->getItemClasses();
}


FACTORY_FUNCTION GraphicsScene::getFactoryFunction(const QString& itemClass) {
    return itemFactory->getFactoryFunction(itemClass);
}
