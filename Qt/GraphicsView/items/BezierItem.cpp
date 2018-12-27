#include <QDebug>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QStyleOptionGraphicsItem>
#include <QCursor>
#include <QPointF>
#include <QXmlStreamWriter>

#include <math.h>

#include "BezierItem.h"
#include "GraphicsSheet.h"
#include "Interactor.h"

BezierItem::BezierItem() : QGraphicsPathItem(0) {
    setAcceptHoverEvents(true); // TODO: Only necessary when the item is selected!
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemClipsToShape, true);

    // p1 = QPointF(0,0);
    p2 = QPointF(20, 0);
    c1 = QPointF(10,-5);
    c2 = QPointF(15,5);

    path = QPainterPath();
    path.cubicTo(c1, c2, p2);
    setPath(path);
}

#if 0
BezierItem::BezierItem(const QPointF& pos1, const QPointF& pos2, QGraphicsItem * parent) :
    QGraphicsLineItem(0, 0, pos2.x() - pos1.x(), pos2.y() - pos1.y(), parent) {

    setPos(pos1);

    setAcceptHoverEvents(true); // TODO: Only necessary when the item is selected!
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemClipsToShape, true);
}
#endif

QGraphicsItem* BezierItem::create() {
    return new BezierItem();
}


AbstractEditHandle BezierItem::getEditHandle(GraphicsSheet* view, const QPointF& scenePos, AbstractEditHandle enabledHandles){
    if (isSelected()) {
        QSizeF handleSize = view->getHandleSize();
        QPointF pos = mapFromScene(scenePos);
        QRectF area = QRectF(pos.x() - handleSize.width() / 2, pos.y() - handleSize.height() / 2,
                             handleSize.width(), handleSize.height());

        if ( (enabledHandles & P2HandleMask) && area.contains(p2)) {
            return P2Handle;
        } else if ( (enabledHandles & P1HandleMask) && area.contains(0, 0)) {
            return P1Handle;
        } else if ( (enabledHandles & C1HandleMask) && area.contains(c1)) {
            return C1Handle;
        } else if ( (enabledHandles & C2HandleMask) && area.contains(c2)) {
            return C2Handle;
        } else if ( (enabledHandles & MoveHandleMask) && shape().contains(pos.toPoint())) {
            return MoveHandle;
        }
    }
    return NoHandle;
}


AbstractEditHandle BezierItem::getNewHandle() {
    return P2Handle;
}


void BezierItem::paintHandles(GraphicsSheet* view, QPainter * painter, AbstractEditHandle enabledHandles){
    if (isSelected()) {
        QSizeF handleSize = view->getHandleSize();

        // Paint the handles
        painter->setBrush(Qt::green);
        painter->setPen(QPen(Qt::black, 0, Qt::SolidLine));

        if (enabledHandles & P2HandleMask)
            painter->drawRect(QRectF(p2.x() - handleSize.width() / 2,
                                     p2.y() - handleSize.height() / 2,
                                     handleSize.width(), handleSize.height()));
        if (enabledHandles & P1HandleMask)
            painter->drawRect(QRectF(-handleSize.width() / 2,
                                     -handleSize.height() / 2,
                                     handleSize.width(), handleSize.height()));

        painter->setBrush(Qt::yellow);

        if (enabledHandles & C1HandleMask) {
            painter->setPen(QPen(Qt::green, 0, Qt::DashLine));
            painter->drawLine(QPointF(0, 0), c1);

            painter->setPen(QPen(Qt::black, 0, Qt::SolidLine));
            painter->drawRect(QRectF(c1.x() - handleSize.width() / 2,
                                     c1.y() - handleSize.height() / 2,
                                     handleSize.width(), handleSize.height()));
        }
        if (enabledHandles & C1HandleMask) {
            painter->setPen(QPen(Qt::green, 0, Qt::DashLine));
            painter->drawLine(p2, c2);

            painter->setPen(QPen(Qt::black, 0, Qt::SolidLine));
            painter->drawRect(QRectF(c2.x() - handleSize.width() / 2,
                                     c2.y() - handleSize.height() / 2,
                                     handleSize.width(), handleSize.height()));
        }
    }

}


QPointF BezierItem::getHandleOffset(AbstractEditHandle editHandle, const QPointF& scenePos) {
    QPointF ref = scenePos;


    switch(editHandle) {
        case P1Handle :
            ref = mapToScene(0, 0);
            break;

        case P2Handle :
            ref = mapToScene(p2);
            break;

        case C1Handle :
            ref = mapToScene(c1);
            break;


        case C2Handle :
            ref = mapToScene(c2);
            break;

        case MoveHandle:
            ref = pos();
            break;
    }

    return ref - scenePos;
}


void BezierItem::moveHandle(AbstractEditHandle editHandle, const QPointF& scenePos){
    switch(editHandle) {
        case BezierItem::P1Handle : {
                    QPointF P1 = scenePos;

                    QPointF P2 = mapToScene(p2);
                    setPos(P1);
                    p2 = mapFromScene(P2);

                    path = QPainterPath();
                    path.cubicTo(c1, c2, p2);
                    setPath(path);
                }
                break;

        case BezierItem::P2Handle : {
                    QPointF P2 = scenePos;
                    p2 = mapFromScene(P2);

                    path = QPainterPath();
                    path.cubicTo(c1, c2, p2);
                    setPath(path);
                }
                break;

        case BezierItem::C1Handle : {
                    c1 = mapFromScene(scenePos);

                    path = QPainterPath();
                    path.cubicTo(c1, c2, p2);
                    setPath(path);
                }
                break;

        case BezierItem::C2Handle : {
                    c2 = mapFromScene(scenePos);

                    path = QPainterPath();
                    path.cubicTo(c1, c2, p2);
                    setPath(path);
                }
                break;

        case BezierItem::MoveHandle:
                setPos(scenePos.x(), scenePos.y());
                break;
    }
}


void BezierItem::setCursor(GraphicsSheet* theView, unsigned int /*EditHandle*/ handle){
    switch(handle) {
        case BezierItem::P1Handle :
        case BezierItem::P2Handle :
        case BezierItem::C1Handle :
        case BezierItem::C2Handle :
                theView->setCursor(Qt::SizeAllCursor);
                break;

        case BezierItem::MoveHandle :
                theView->setCursor(Qt::OpenHandCursor);
                break;

        default :
                theView->setCursor(Qt::ArrowCursor);
                break;
    }

}


QPointF BezierItem::getNearestEdge(GraphicsSheet* theView, const QPointF& scenePos) {
    QPointF pos = mapFromScene(scenePos);

//    qreal l1 = (pos - line().p1()).manhattanLength();
//    qreal l2 = (pos - line().p2()).manhattanLength();

    QPointF result;
//    if (l1 < l2) {
//        result = line().p1();
 //   } else {
 //       result = line().p2();
 //  }

    return mapToScene(result);
}


void BezierItem::paintSelectedBorder(GraphicsSheet* view, QPainter * painter){
    Q_UNUSED(view);

    if (isSelected()) {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(Qt::green, 0, Qt::DashLine));

        painter->drawPath(path);
    }

}


void BezierItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * style, QWidget *widget) {
    // reset the selected state when painting - this is a workaround
    // to avoid that QGraphicsBezierItem::paint draws a strange border around the
    // line when it is selected (only on vertical and horizontal lines!)
    QStyleOptionGraphicsItem option2 = *style;
    option2.state = 0;

    QGraphicsPathItem::paint(painter, &option2, widget);
}

QRectF BezierItem::boundingRect() const {
     QRectF result = QGraphicsPathItem::boundingRect();
     return QRectF(result.x() - 2, result.y() - 2, result.width() + 4, result.height() + 4);
}


QPainterPath BezierItem::shape () const {
    return QGraphicsPathItem::shape();
#if 0
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
#endif
}


void BezierItem::writeExternal(QXmlStreamWriter& writer) {
    writer.writeStartElement("BezierItem");

    QPointF P1 = pos();
    QPointF P2 = mapToScene(p2);
    QPointF C1 = mapToScene(c1);
    QPointF C2 = mapToScene(c2);

    writer.writeAttribute("x1", QString::number(P1.x()));
    writer.writeAttribute("y1", QString::number(P1.y()));
    writer.writeAttribute("x2", QString::number(P2.x()));
    writer.writeAttribute("y2", QString::number(P2.y()));
    writer.writeAttribute("c1x", QString::number(C1.x()));
    writer.writeAttribute("c1y", QString::number(C1.y()));
    writer.writeAttribute("c2x", QString::number(C2.x()));
    writer.writeAttribute("c2y", QString::number(C2.y()));

    writer.writeAttribute("color", pen().color().name());
    writer.writeAttribute("style", QString::number(pen().style()));
    writer.writeAttribute("width", QString::number(pen().widthF()));

    writer.writeEndElement();
}


void BezierItem::readExternal(QXmlStreamReader& reader) {
    qreal x1 = reader.attributes().value("x1").toFloat();
    qreal y1 = reader.attributes().value("y1").toFloat();
    qreal x2 = reader.attributes().value("x2").toFloat();
    qreal y2 = reader.attributes().value("y2").toFloat();
    qreal c1x = reader.attributes().value("c1x").toFloat();
    qreal c1y = reader.attributes().value("c1y").toFloat();
    qreal c2x = reader.attributes().value("c2x").toFloat();
    qreal c2y = reader.attributes().value("c2y").toFloat();

    QColor color = QColor(reader.attributes().value("color").toString());
    Qt::PenStyle style = static_cast<Qt::PenStyle>(reader.attributes().value("style").toInt());
    qreal width = reader.attributes().value("width").toFloat();

    QPointF itemPos = QPointF(x1, y1);
    setPos(itemPos);

    p2 = mapFromScene(QPointF(x2, y2));
    c1 = mapFromScene(QPointF(c1x, c1y));
    c2 = mapFromScene(QPointF(c2x, c2y));

    path = QPainterPath();
    path.cubicTo(c1, c2, p2);
    setPath(path);

    setPen(QPen(color, width, style));
}
