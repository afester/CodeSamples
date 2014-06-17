
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QStyleOptionGraphicsItem>
#include <QCursor>
#include <QPointF>
#include <QXmlStreamWriter>

#include <math.h>

#include "LineItem.h"
#include "GraphicsSheet.h"
#include "Interactor.h"

LineItem::LineItem() : QGraphicsLineItem(0) {
    setAcceptHoverEvents(true); // TODO: Only necessary when the item is selected!
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemClipsToShape, true);
}


LineItem::LineItem(const QPointF& pos1, const QPointF& pos2, QGraphicsItem * parent) :
    QGraphicsLineItem(0, 0, pos2.x() - pos1.x(), pos2.y() - pos1.y(), parent) {

    setPos(pos1);

    setAcceptHoverEvents(true); // TODO: Only necessary when the item is selected!
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemClipsToShape, true);
}


QGraphicsItem* LineItem::create() {
    return new LineItem();
}


AbstractEditHandle LineItem::getEditHandle(GraphicsSheet* view, const QPointF& scenePos, AbstractEditHandle enabledHandles){
    if (isSelected()) {
        QSizeF handleSize = view->getHandleSize();
        QPointF pos = mapFromScene(scenePos);
        QRectF area = QRectF(pos.x() - handleSize.width() / 2, pos.y() - handleSize.height() / 2,
                             handleSize.width(), handleSize.height());

        if ( (enabledHandles & P2HandleMask) && area.contains(line().p2())) {
            return P2Handle;
        } else if ( (enabledHandles & P1HandleMask) && area.contains(line().p1())) {
            return P1Handle;
        } else if ( (enabledHandles & MoveHandleMask) && shape().contains(pos.toPoint())) {
            return MoveHandle;
        }
    }

    return NoHandle;
}


void LineItem::paintHandles(GraphicsSheet* view, QPainter * painter, AbstractEditHandle enabledHandles){
    if (isSelected()) {
        QSizeF handleSize = view->getHandleSize();

        // Paint the handles
        painter->setBrush(Qt::green);
        painter->setPen(QPen(Qt::black, 0, Qt::SolidLine));

        if (enabledHandles & P2HandleMask)
            painter->drawRect(QRectF(line().p2().x() - handleSize.width() / 2,
                                     line().p2().y() - handleSize.height() / 2,
                                     handleSize.width(), handleSize.height()));
        if (enabledHandles & P1HandleMask)
            painter->drawRect(QRectF(line().p1().x() - handleSize.width() / 2,
                                     line().p1().y() - handleSize.height() / 2,
                                     handleSize.width(), handleSize.height()));
    }

}


QPointF LineItem::getHandleOffset(AbstractEditHandle editHandle, const QPointF& scenePos) {
    QPointF ref = scenePos;


    switch(editHandle) {
        case P1Handle :
            ref = mapToScene(line().p1());
            break;

        case P2Handle :
            ref = mapToScene(line().p2());
            break;

        case MoveHandle:
            ref = pos();
            break;
    }

    return ref - scenePos;
}


void LineItem::moveHandle(AbstractEditHandle editHandle, const QPointF& scenePos){

    switch(editHandle) {
        case LineItem::P1Handle : {
                    QPointF P1 = scenePos;
                    QPointF P2 = mapToScene(line().p2());

                    QPointF pos = QPointF(qMin(P1.x(), P2.x()),
                                          qMin(P1.y(), P2.y()));
                    setPos(pos);

                    QLineF l;
                    l.setP1(mapFromScene(P1));
                    l.setP2(mapFromScene(P2));
                    setLine(l);
                }
                break;

        case LineItem::P2Handle : {
                    QPointF P1 = mapToScene(line().p1());
                    QPointF P2 = scenePos;

                    QPointF pos = QPointF(qMin(P1.x(), P2.x()),
                                          qMin(P1.y(), P2.y()));
                    setPos(pos);

                    QLineF l;
                    l.setP1(mapFromScene(P1));
                    l.setP2(mapFromScene(P2));
                    setLine(l);

                }
                break;

        case LineItem::MoveHandle:
                setPos(scenePos.x(), scenePos.y());
                break;
    }

}


void LineItem::setCursor(GraphicsSheet* theView, unsigned int /*EditHandle*/ handle){
    switch(handle) {
        case LineItem::P1Handle :
        case LineItem::P2Handle :
                theView->setCursor(Qt::SizeAllCursor);
                break;

        case LineItem::MoveHandle :
                theView->setCursor(Qt::OpenHandCursor);
                break;

        default :
                theView->setCursor(Qt::ArrowCursor);
                break;
    }

}


QPointF LineItem::getNearestEdge(GraphicsSheet* theView, const QPointF& scenePos) {
    QPointF pos = mapFromScene(scenePos);

    qreal l1 = (pos - line().p1()).manhattanLength();
    qreal l2 = (pos - line().p2()).manhattanLength();

    QPointF result;
    if (l1 < l2) {
        result = line().p1();
    } else {
        result = line().p2();
    }

    return mapToScene(result);
}


void LineItem::paintSelectedBorder(GraphicsSheet* view, QPainter * painter){
    Q_UNUSED(view);

    if (isSelected()) {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(Qt::green, 0, Qt::DashLine));

        painter->drawLine(line());
    }

}


void LineItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * style, QWidget *widget) {
    // reset the selected state when painting - this is a workaround
    // to avoid that QGraphicsLineItem::paint draws a strange border around the
    // line when it is selected (only on vertical and horizontal lines!)
    QStyleOptionGraphicsItem option2 = *style;
    option2.state = 0;

    QGraphicsLineItem::paint(painter, &option2, widget);
}

QRectF LineItem::boundingRect() const {
     QRectF result = QGraphicsLineItem::boundingRect();
     return QRectF(result.x() - 2, result.y() - 2, result.width() + 4, result.height() + 4);
}


QPainterPath LineItem::shape () const {
    // calculate a "ribbon" around the line which makes selecting
    // the line easier. TODO: Any better algorithm?

    const qreal ribbonWidth = 2.0;

    QLineF uv1 = line().normalVector();
    uv1.setLength(ribbonWidth);
    QPointF P1 = uv1.p2();
    uv1.setLength(-ribbonWidth);
    QPointF P3 = uv1.p2();

    QLineF l2(line().p2(), line().p1());
    QLineF uv2 = l2.normalVector();
    uv2.setLength(ribbonWidth);
    QPointF P4 = uv2.p2();
    uv2.setLength(-ribbonWidth);
    QPointF P2 = uv2.p2();

    QPainterPath path;
    path.moveTo(P1);
    path.lineTo(P2);
    path.lineTo(P4);
    path.lineTo(P3);
    path.closeSubpath();

    return path;
}


void LineItem::writeExternal(QXmlStreamWriter& writer) {
    writer.writeStartElement("LineItem");


    QPointF p1 = mapToScene(line().p1());
    QPointF p2 = mapToScene(line().p2());

    writer.writeAttribute("x1", QString::number(p1.x()));
    writer.writeAttribute("y1", QString::number(p1.y()));
    writer.writeAttribute("x2", QString::number(p2.x()));
    writer.writeAttribute("y2", QString::number(p2.y()));
    writer.writeAttribute("color", pen().color().name());
    writer.writeAttribute("style", QString::number(pen().style()));
    writer.writeAttribute("width", QString::number(pen().widthF()));

    writer.writeEndElement();
}


void LineItem::readExternal(QXmlStreamReader& reader) {
    qreal x1 = reader.attributes().value("x1").toFloat();
    qreal y1 = reader.attributes().value("y1").toFloat();
    qreal x2 = reader.attributes().value("x2").toFloat();
    qreal y2 = reader.attributes().value("y2").toFloat();

    QColor color = QColor(reader.attributes().value("color").toString());
    Qt::PenStyle style = static_cast<Qt::PenStyle>(reader.attributes().value("style").toInt());
    qreal width = reader.attributes().value("width").toFloat();

    QPointF itemPos(qMin(x1, x2), qMin(y1, y2));
    QPointF p1 = QPointF(x1, y1) - itemPos;
    QPointF p2 = QPointF(x2, y2) - itemPos;

    setPos(itemPos);
    setLine(QLineF(p1, p2));
    setPen(QPen(color, width, style));
}
