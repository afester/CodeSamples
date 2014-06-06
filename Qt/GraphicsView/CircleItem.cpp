#include <QDebug>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QCursor>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QPointF>
#include <QXmlStreamWriter>

#include <math.h>

#include "CircleItem.h"
#include "GraphicsSheet.h"
#include "Interactor.h"

#if 0
void ItemVisitor::visitSelectedItems(QGraphicsScene* scene) {
    QList<QGraphicsItem*> selectedItems = scene->selectedItems();
    QGraphicsItem* item;
    foreach(item, selectedItems) {
        CircleItem* theItem = dynamic_cast<CircleItem*>(item);
        if (theItem) {
            theItem->accept(*this);
        }
    }

}

void ItemVisitor::visit(EditableTextItem* ) const {
}

void ItemVisitor::visit(EditableImageframeItem* ) const {
}
#endif


CircleItem::CircleItem() : QGraphicsEllipseItem(0) {
    setAcceptHoverEvents(true); // TODO: Only necessary when the item is selected!
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemClipsToShape, true);
}


CircleItem::CircleItem(const QPointF& pos, QGraphicsItem * parent) :
    QGraphicsEllipseItem(-5, -5, 10, 10, parent) {

    setPos(pos.x(), pos.y());

    setAcceptHoverEvents(true); // TODO: Only necessary when the item is selected!
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemClipsToShape, true);

    radHandle = QPointF(5, 0);
}


CircleItem::CircleItem(const QPointF& cpos, qreal radius, QGraphicsItem * parent) :
    QGraphicsEllipseItem(-radius, -radius, 2*radius, 2*radius, parent){

    setPos(cpos.x(), cpos.y());

    setAcceptHoverEvents(true); // TODO: Only necessary when the item is selected!
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemClipsToShape, true);

    radHandle = QPointF(radius, 0);
}


QGraphicsItem* CircleItem::create() {
    return new CircleItem();
}


QRectF CircleItem::boundingRect() const {
    QRectF result = QGraphicsEllipseItem::boundingRect();
    return QRectF(result.x() - 2, result.y() - 2, result.width() + 4, result.height() + 4);
}


QPainterPath CircleItem::shape () const {
    QPainterPath path;

    QRectF r = rect();
    r.adjust(-2,-2,2,2);
    path.addRect(r);

    return path;
}


AbstractEditHandle CircleItem::getEditHandle(GraphicsSheet* view, const QPointF& scenePos, unsigned int /*EditHandles */ enabledHandles) {
    if (isSelected()) {
        QSizeF handleSize = view->getHandleSize();
        QPointF pos = mapFromScene(scenePos);
        QRectF area = QRectF(pos.x() - handleSize.width() / 2, pos.y() - handleSize.height() / 2,
                             handleSize.width(), handleSize.height());

        if ( (enabledHandles & RadHandleMask) && area.contains(radHandle)) {
            return RadHandle;
        } else if ( (enabledHandles & MoveHandleMask) && rect().contains(pos.toPoint())) {
            return MoveHandle;
        }
    }

    return NoHandle;
}


void CircleItem::paintSelectedBorder(GraphicsSheet* view, QPainter * painter) {
    Q_UNUSED(view);

    if (isSelected()) {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(Qt::green, 0, Qt::DashLine));

        // draw the shape of the circle
        painter->drawEllipse(rect());

        // draw a radius line
        painter->drawLine(QPointF(0, 0), radHandle);

        // draw a center cross
        QSizeF handleSize = view->getHandleSize() / 2;
        painter->setPen(QPen(Qt::green, 0, Qt::SolidLine));
        painter->drawLine(QPointF(-handleSize.width(), 0),
                          QPointF(handleSize.width(), 0));
        painter->drawLine(QPointF(0, -handleSize.height()),
                          QPointF(0, handleSize.height()));
    }
}


void CircleItem::paintHandles(GraphicsSheet* view, QPainter * painter, AbstractEditHandle enabledHandles) {
    if (isSelected()) {
        // Paint the handles
		painter->setBrush(Qt::green);
		painter->setPen(QPen(Qt::black, 0, Qt::SolidLine));	// Qt5: need to explicitly set width to 0, even though the docs claim other

		QSizeF handleSize = view->getHandleSize();
		if (enabledHandles & RadHandleMask)
		    painter->drawRect(QRectF(radHandle.x() - handleSize.width()/2,
		                             radHandle.y() - handleSize.height()/2,
		                             handleSize.width(), handleSize.height()));
    }
}


void CircleItem::moveRadHandle(const QPointF& scenePos) {
    QPointF posInItem = mapFromScene(scenePos);

    qreal dx = posInItem.x();
    qreal dy = posInItem.y();
    qreal radius = sqrt( (dx * dx) + (dy * dy));
    if (radius < 5) {
        radius = 5;
    } else {
        radHandle = posInItem;
    }

    setRect(QRectF(-radius, -radius, 2*radius, 2*radius));
}


QPointF CircleItem::getHandleOffset(AbstractEditHandle editHandle, const QPointF& scenePos) {
    QPointF ref = scenePos;

    switch(editHandle) {
        case CircleItem::RadHandle :
            ref = mapToScene(radHandle);
            break;

        case CircleItem::MoveHandle:
            ref = pos();
            break;
    }

    return ref - scenePos;
}


void CircleItem::moveHandle(AbstractEditHandle editHandle, const QPointF& scenePos) {

    switch(editHandle) {
        case CircleItem::RadHandle :
                moveRadHandle(scenePos);
                break;

        case CircleItem::MoveHandle:
                setPos(scenePos.x(), scenePos.y());
                break;
    }
}


void CircleItem::setCursor(GraphicsSheet* theView, AbstractEditHandle handle) {

    switch(handle) {
        case CircleItem::RadHandle :
                theView->setCursor(Qt::SizeAllCursor);
                break;

        case CircleItem::MoveHandle :
                theView->setCursor(Qt::OpenHandCursor);
                break;

        default :
                theView->setCursor(Qt::ArrowCursor);
                break;
    }
}


QPointF CircleItem::getNearestEdge(GraphicsSheet* view, const QPointF& scenePos) {
#if 0
    QPointF pos = mapFromScene(scenePos);

    // topLeft, topRight, bottomRight, bottomLeft
    // QPointF p1 = scenePos - topLeft;

    calculateHandles(view);

    qreal min = (pos - topLeft).manhattanLength();
    qreal l2 = (pos - topRight).manhattanLength();
    qreal l3 = (pos - bottomRight).manhattanLength();
    qreal l4 = (pos - bottomLeft).manhattanLength();
    // qDebug() << min << ", " << l2<<", "<<l3<<", "<<l4;

    QPointF result = topLeft;
    if (l2 < min) {
        result = topRight;
        min = l2;
    }
    if (l3 < min) {
        result = bottomRight;
        min = l3;
    }
    if (l4 < min) {
        result = bottomLeft;
    }

    return mapToScene(result);
#endif
    return QPointF(0,0);
}


void CircleItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * style, QWidget *widget) {
    // reset the selected state when painting - this is a workaround
    // to avoid that QGraphicsCircleItem::paint draws the selection rectangle
    QStyleOptionGraphicsItem option2 = *style;
    option2.state = 0;
    QGraphicsEllipseItem::paint(painter, &option2, widget);

#if 0
	GraphicsSheet* requestingView = dynamic_cast<GraphicsSheet*>(widget ? widget->parent() : 0);
	if (requestingView) {
		Interactor* inter = requestingView->getInteractor();
		if (inter) {
			inter->paintDecorations(this, painter);
		}
	}
#endif
}


void CircleItem::writeExternal(QXmlStreamWriter& writer) {
    writer.writeStartElement("CircleItem");

    writer.writeAttribute("xpos", QString::number(x()));
    writer.writeAttribute("ypos", QString::number(y()));
    writer.writeAttribute("radius", QString::number(rect().width()/2));

    writer.writeAttribute("background-color", brush().color().name());
    writer.writeAttribute("border-color", pen().color().name());
    writer.writeAttribute("border-style", QString::number(pen().style()));
    writer.writeAttribute("border-width", QString::number(pen().widthF()));

    writer.writeEndElement();
}


void CircleItem::readExternal(QXmlStreamReader& reader) {
    qreal xpos = reader.attributes().value("xpos").toFloat();
    qreal ypos = reader.attributes().value("ypos").toFloat();
    qreal radius = reader.attributes().value("radius").toFloat();

    QColor color = QColor(reader.attributes().value("background-color").toString());
    QColor borderColor = QColor(reader.attributes().value("border-color").toString());
    Qt::PenStyle borderStyle = static_cast<Qt::PenStyle>(reader.attributes().value("border-style").toInt());
    qreal borderWidth = reader.attributes().value("border-width").toFloat();

    setPos(xpos, ypos);
    setRect(-radius, -radius, radius*2, radius*2);
    setPen(QPen(borderColor, borderWidth, borderStyle));
    setBrush(QBrush(color));

    radHandle = QPointF(radius, 0);
}
