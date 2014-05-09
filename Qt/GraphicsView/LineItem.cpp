
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QStyleOptionGraphicsItem>
#include <QCursor>
#include <QPointF>

#include <math.h>

#include "LineItem.h"
#include "GraphicsSheet.h"
#include "Interactor.h"


LineItem::LineItem(const QPointF& pos1, const QPointF& pos2, QGraphicsItem * parent) :
    QGraphicsLineItem(0, 0, pos2.x() - pos1.x(), pos2.y() - pos1.y(), parent) {

    setPos(pos1);

    setAcceptHoverEvents(true); // TODO: Only necessary when the item is selected!
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemClipsToShape, true);
}


void LineItem::calculateHandles(GraphicsSheet* view) {
    const qreal handleSize = 8.0;   // TODO: read from view property

    const qreal hsize = handleSize / view->transform().m11();
    const qreal vsize = handleSize / view->transform().m22();

    // Initialize handles
    const qreal hsizeHalf = hsize / 2;
    const qreal vsizeHalf = hsize /2;

    p1Handle = QRectF(line().p1().x() - hsizeHalf, line().p1().y() - vsizeHalf, hsize, vsize);
    p2Handle = QRectF(line().p2().x() - hsizeHalf, line().p2().y() - vsizeHalf, hsize, vsize);
}


unsigned int /*EditHandle*/ LineItem::getEditHandle(GraphicsSheet* view, const QPointF& scenePos, unsigned int /*EditHandles*/ enabledHandles){
    if (isSelected()) {
        calculateHandles(view);
        QPointF pos = mapFromScene(scenePos);

        if ( (enabledHandles & P2HandleMask) && p2Handle.contains(pos.toPoint())) {
            return P2Handle;
        } else if ( (enabledHandles & P1HandleMask) && p1Handle.contains(pos.toPoint())) {
            return P1Handle;
        } else if ( (enabledHandles & MoveHandleMask) && shape().contains(pos.toPoint())) {
            return MoveHandle;
        }
    }

    return NoHandle;
}

void LineItem::paintHandles(GraphicsSheet* view, QPainter * painter, unsigned int/*EditHandles */ enabledHandles){
    if (isSelected()) {
        calculateHandles(view);

        // Paint the handles
        painter->setBrush(Qt::green);
        painter->setPen(QPen(Qt::black, 0, Qt::SolidLine));

        if (enabledHandles & P2HandleMask)
            painter->drawRect(p2Handle);
        if (enabledHandles & P1HandleMask)
            painter->drawRect(p1Handle);
    }

}


QSizeF LineItem::getHandleOffset(unsigned int editHandle, const QPointF& scenePos) {
    switch(editHandle) {
        case P1Handle :
                return QSizeF(0, 0); // QSizeF(theFrame->x() - scenePos.x(), 0);
                //positionIndicator = QPoint(theFrame->x(), -1);

        case P2Handle :
                return QSizeF(0, 0); // QSizeF(theFrame->x() + theFrame->rect().width() - scenePos.x(), 0);
                //positionIndicator = QPoint(theFrame->x() + theFrame->rect().width(), -1);
            //                               theFrame->y() + theFrame->rect().height());

        case MoveHandle:
                return QSize(x() - scenePos.x(), y() - scenePos.y());
    }

    return QSize();
}


void LineItem::moveHandle(unsigned int /*EditHandle*/ editHandle, const QPointF& scenePos){

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


void LineItem::paintSelectedBorder(GraphicsSheet* view, QPainter * painter){
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
