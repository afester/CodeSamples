#include <QDebug>
#include <QPainter>
#include <QGraphicsScene>
#include <QCursor>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QPointF>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#define _USE_MATH_DEFINES
#include <math.h>

#include "DefaultFrame.h"
#include "GraphicsSheet.h"
#include "Interactor.h"

#if 0
void ItemVisitor::visitSelectedItems(QGraphicsScene* scene) {
    QList<QGraphicsItem*> selectedItems = scene->selectedItems();
    QGraphicsItem* item;
    foreach(item, selectedItems) {
        DefaultFrame* theItem = dynamic_cast<DefaultFrame*>(item);
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


DefaultFrame::DefaultFrame() : QGraphicsRectItem(0) {
    setAcceptHoverEvents(true); // TODO: Only necessary when the item is selected!
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemClipsToShape, true);
}

DefaultFrame::DefaultFrame(const QPointF& pos, QGraphicsItem * parent) :
    QGraphicsRectItem(0, 0, 10, 10, parent) {

    setPos(pos.x(), pos.y());

    setAcceptHoverEvents(true); // TODO: Only necessary when the item is selected!
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemClipsToShape, true);
}


DefaultFrame::DefaultFrame(const QRectF & rect, QGraphicsItem * parent) :
    QGraphicsRectItem(0, 0, rect.width(), rect.height(), parent){

    setPos(rect.x(), rect.y());

    setAcceptHoverEvents(true); // TODO: Only necessary when the item is selected!
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemClipsToShape, true);
}


QGraphicsItem* DefaultFrame::create() {
    return new DefaultFrame();
}


void DefaultFrame::writeExternal(QXmlStreamWriter& writer) {
    writer.writeStartElement("DefaultFrame");

    writer.writeAttribute("xpos", QString::number(x()));
    writer.writeAttribute("ypos", QString::number(y()));
    writer.writeAttribute("width", QString::number(rect().width()));
    writer.writeAttribute("height", QString::number(rect().height()));
    writer.writeAttribute("rotation", QString::number(rotation()));

    writer.writeAttribute("background-color", brush().color().name());
    writer.writeAttribute("border-color", pen().color().name());
    writer.writeAttribute("border-style", QString::number(pen().style()));
    writer.writeAttribute("border-width", QString::number(pen().widthF()));

    writer.writeEndElement();
}


void DefaultFrame::readExternal(QXmlStreamReader& reader) {
    qreal xpos = reader.attributes().value("xpos").toFloat();
    qreal ypos = reader.attributes().value("ypos").toFloat();
    qreal width = reader.attributes().value("width").toFloat();
    qreal height = reader.attributes().value("height").toFloat();
    qreal rotation = reader.attributes().value("rotation").toFloat();

    QColor color = QColor(reader.attributes().value("background-color").toString());
    QColor borderColor = QColor(reader.attributes().value("border-color").toString());
    Qt::PenStyle borderStyle = static_cast<Qt::PenStyle>(reader.attributes().value("border-style").toInt());
    qreal borderWidth = reader.attributes().value("border-width").toFloat();

    setPos(xpos, ypos);
    setRect(0, 0, width, height);
    setRotation(rotation);
    setTransformOriginPoint(rect().width()/2, rect().height()/2);
    setPen(QPen(borderColor, borderWidth, borderStyle));
    setBrush(QBrush(color));
}


void DefaultFrame::calculateHandles(GraphicsSheet* view) {
    const qreal width = rect().width();
    const qreal height = rect().height();
    const qreal xhalf = width / 2;
    const qreal yhalf = height / 2;

    rotationHandle = QPointF(xhalf, 2 * view->getHandleSize().height());
    topLeft        = QPointF(0, 0);
    topRight       = QPointF(width, 0);
    bottomLeft     = QPointF(0, height);
    bottomRight    = QPointF(width, height);
    top            = QPointF(xhalf, 0);
    bottom         = QPointF(xhalf, height);
    left           = QPointF(0, yhalf);
    right          = QPointF(width, yhalf);
}


QRectF DefaultFrame::boundingRect() const {
    QRectF result = QGraphicsRectItem::boundingRect();
    return QRectF(result.x() - 2, result.y() - 2, result.width() + 4, result.height() + 4);
}


QPainterPath DefaultFrame::shape () const {
    QPainterPath path;

    QRectF r = rect();
    r.adjust(-2,-2,2,2);
    path.addRect(r);

    return path;
}


AbstractEditHandle DefaultFrame::getEditHandle(GraphicsSheet* view, const QPointF& scenePos, unsigned int /*EditHandles */ enabledHandles) {
    if (isSelected()) {
        calculateHandles(view);

        QSizeF handleSize = view->getHandleSize();
        QPointF pos = mapFromScene(scenePos);
        QRectF area = QRectF(pos.x() - handleSize.width() / 2, pos.y() - handleSize.height() / 2,
                             handleSize.width(), handleSize.height());

        if ( (enabledHandles & RotationHandleMask) && area.contains(rotationHandle)) { // rotationHandle.contains(pos.toPoint())) {
            return RotationHandle;
        } else if ( (enabledHandles & TopLeftHandleMask) && area.contains(topLeft)) { //  topLeft.contains(pos.toPoint())) {
        	return TopLeftHandle;
        } else if ( (enabledHandles & TopHandleMask) && area.contains(top)) { // top.contains(pos.toPoint())) {
        	return TopHandle;
        } else if ( (enabledHandles & TopRightHandleMask) && area.contains(topRight)) { // .contains(pos.toPoint())) {
        	return TopRightHandle;
        } else if ( (enabledHandles & RightHandleMask) && area.contains(right)) { // .contains(pos.toPoint())) {
        	return RightHandle;
        } else if ( (enabledHandles & BottomRightHandleMask) && area.contains(bottomRight)) { // .contains(pos.toPoint())) {
        	return BottomRightHandle;
        } else if ( (enabledHandles & BottomHandleMask) && area.contains(bottom)) { // .contains(pos.toPoint())) {
        	return BottomHandle;
        } else if ( (enabledHandles & BottomLeftHandleMask) && area.contains(bottomLeft)) { // .contains(pos.toPoint())) {
            return BottomLeftHandle;
        } else if ( (enabledHandles & LeftHandleMask) && area.contains(left)) { // .contains(pos.toPoint())) {
        	return LeftHandle;
        } else if ( (enabledHandles & MoveHandleMask) && rect().contains(pos.toPoint())) {
            return MoveHandle;
        }
    }

    return NoHandle;
}


AbstractEditHandle DefaultFrame::getNewHandle() {
    return BottomRightHandle;
}

void DefaultFrame::paintSelectedBorder(GraphicsSheet* view, QPainter * painter) {
    Q_UNUSED(view);

    if (isSelected()) {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(Qt::green, 0, Qt::DashLine));
        painter->drawRect(rect());
    }
}


void DefaultFrame::paintHandles(GraphicsSheet* view, QPainter * painter, AbstractEditHandle enabledHandles) {
    if (isSelected()) {
        calculateHandles(view);

        // Paint the handles
		painter->setBrush(Qt::green);
		painter->setPen(QPen(Qt::black, 0, Qt::SolidLine));	// Qt5: need to explicitly set width to 0, even though the docs claim other

		QSizeF handleSize = view->getHandleSize();
		if (enabledHandles & RotationHandleMask)
			painter->drawEllipse(rotationHandle, handleSize.width() / 2, handleSize.height() / 2);
		if (enabledHandles & TopLeftHandleMask)
			painter->drawRect(QRectF(topLeft.x() - handleSize.width()/2,     topLeft.y() - handleSize.height()/2, handleSize.width(), handleSize.height()));
		if (enabledHandles & TopHandleMask)
		    painter->drawRect(QRectF(top.x() - handleSize.width()/2,         top.y() - handleSize.height()/2, handleSize.width(), handleSize.height()));
		if (enabledHandles & TopRightHandleMask)
            painter->drawRect(QRectF(topRight.x() - handleSize.width()/2,    topRight.y() - handleSize.height()/2, handleSize.width(), handleSize.height()));
		if (enabledHandles & RightHandleMask)
		    painter->drawRect(QRectF(right.x() - handleSize.width()/2,       right.y() - handleSize.height()/2, handleSize.width(), handleSize.height()));
		if (enabledHandles & BottomRightHandleMask)
            painter->drawRect(QRectF(bottomRight.x() - handleSize.width()/2, bottomRight.y() - handleSize.height()/2, handleSize.width(), handleSize.height()));
		if (enabledHandles & BottomHandleMask)
            painter->drawRect(QRectF(bottom.x() - handleSize.width()/2,      bottom.y() - handleSize.height()/2, handleSize.width(), handleSize.height()));
		if (enabledHandles & BottomLeftHandleMask)
            painter->drawRect(QRectF(bottomLeft.x() - handleSize.width()/2,  bottomLeft.y() - handleSize.height()/2, handleSize.width(), handleSize.height()));
		if (enabledHandles & LeftHandleMask)
            painter->drawRect(QRectF(left.x() - handleSize.width()/2,        left.y() - handleSize.height()/2, handleSize.width(), handleSize.height()));
    }
}


void DefaultFrame::moveRotationHandle(const QPointF& scenePos) {
    // calculate center of rectangle (scene coordinates)
    QPointF center = QPointF(pos().x() + rect().width() / 2,
                             pos().y() + rect().height() / 2);

    qreal deltaX = scenePos.x() - center.x();
    qreal deltaY = center.y() - scenePos.y();

    // TODO: RECHECK ANGLE CALCULATION!!!!!! Does not correctly work yet!

    int angle = (atan(deltaX / deltaY) * 360) / (2*M_PI);

    if (deltaY < 0 && deltaX >= 0) {
        angle = 180 - abs(angle);
    } else if (deltaY < 0 && deltaX <= 0) {
        angle = 180 + angle;
    } else if (deltaY >= 0 && deltaX <= 0) {
        angle = 360 + angle;
    }

    // qDebug() << rect() << "/" << center << ", " << deltaX << "/" << deltaY << "=> " << angle;

    // calculate center of rectangle (item coordinates)
    QPointF center2 = QPointF(rect().width() / 2, rect().height() / 2);

    // activate new geometry
    setTransformOriginPoint(center2);
    setRotation(angle);
}


void DefaultFrame::activateGeometry(const QSizeF& newSize, const QPointF& pos1) {
    QPointF P3(newSize.width()/2, newSize.height()/2);

    // transform P1 to the new item position
    QTransform t2;
    t2.translate(pos1.x(), pos1.y());
    t2.rotate(rotation());
    t2.translate(P3.x(), P3.y());
    t2.rotate(-rotation());
    t2.translate(-P3.x(), -P3.y());
    t2.translate(-pos1.x(), -pos1.y());
    QPointF newPos = t2.map(pos1);

    // activate new geometry
    setPos(newPos);
    setRect(QRectF(0, 0, newSize.width(), newSize.height()));
    setTransformOriginPoint(P3);
}


void DefaultFrame::moveTopLeftHandle(const QPointF& P2) {

    QPointF P1 = mapToScene(rect().width(), rect().height());   // fix point

    QTransform t;
    t.rotate(-rotation());
    t.translate(-P1.x(), -P1.y());

    QPointF P2i = t.map(P2);
    QSizeF newSize = QSizeF(-P2i.x(), -P2i.y()).expandedTo(QSizeF(10, 10));

    QTransform t_;
    t_.translate(P1.x(), P1.y());
    t_.rotate(rotation());
    QPointF pos1 = t_.map(QPointF(-newSize.width(), -newSize.height()));

    activateGeometry(newSize, pos1);
}


void DefaultFrame::moveBottomRightHandle(const QPointF& P2) {

    QPointF pos1 = mapToScene(0, 0);      // fix point

    // transform coordinate system so that its center is at P1
    // we need to transform back (from global coordinates into local coordinates),
    // so we need to do the inverse operations (rotate -angle => translate -P1)
    QTransform t;
    t.rotate(-rotation());
    t.translate(-pos1.x(), -pos1.y());

    // Map the fix point into the transformed coordinate system -
    // this will be the new width and height
    QPointF P2i = t.map(P2);
    QSizeF newSize = QSizeF(P2i.x(), P2i.y()).expandedTo(QSizeF(10, 10));

    activateGeometry(newSize, pos1);
}


void DefaultFrame::moveTopRightHandle(const QPointF& P2) {

    QPointF P1 = mapToScene(0, rect().height());    // fix point

    QTransform t;
    t.rotate(-rotation());
    t.translate(-P1.x(), -P1.y());

    QPointF P2i = t.map(P2);
    QSizeF newSize = QSizeF(P2i.x(), -P2i.y()).expandedTo(QSizeF(10, 10));

    QTransform t_;
    t_.translate(P1.x(), P1.y());
    t_.rotate(rotation());
    QPointF pos1 = t_.map(QPointF(0, -newSize.height()));

    activateGeometry(newSize, pos1);
}


void DefaultFrame::moveBottomLeftHandle(const QPointF& P2) {

    QPointF P1 = mapToScene(rect().width(), 0);   // fix point

    QTransform t;
    t.rotate(-rotation());
    t.translate(-P1.x(), -P1.y());

    QPointF P2i = t.map(P2);
    QSizeF newSize = QSizeF(-P2i.x(), P2i.y()).expandedTo(QSizeF(10, 10));

    QTransform t_;
    t_.translate(P1.x(), P1.y());
    t_.rotate(rotation());
    QPointF pos1 = t_.map(QPointF(-newSize.width(), 0));

    activateGeometry(newSize, pos1);
}


void DefaultFrame::moveRightHandle(const QPointF& scenePos) {

    QPointF pos1 = mapToScene(0, 0);    // fix point

    QPointF posInItem = mapFromScene(scenePos);
    QSizeF newSize = QSizeF(posInItem.x(), rect().height()).expandedTo(QSizeF(10, 10));

    activateGeometry(newSize, pos1);
}


void DefaultFrame::moveBottomHandle(const QPointF& scenePos) {

    QPointF pos1 = mapToScene(0, 0);    // fix point

    QPointF posInItem = mapFromScene(scenePos);
    QSizeF newSize = QSizeF(rect().width(), posInItem.y()).expandedTo(QSizeF(10, 10));

    activateGeometry(newSize, pos1);
}


void DefaultFrame::moveTopHandle(const QPointF& P2) {

    QPointF P1 = mapToScene(0, rect().height());    // fix point

    QTransform t;
    t.rotate(-rotation());
    t.translate(-P1.x(), -P1.y());

    QPointF P2i = t.map(P2);
    QSizeF newSize = QSizeF(rect().width(), -P2i.y()).expandedTo(QSizeF(10, 10));

    QTransform t_;
    t_.translate(P1.x(), P1.y());
    t_.rotate(rotation());
    QPointF pos1 = t_.map(QPointF(0, -newSize.height()));

    activateGeometry(newSize, pos1);
}


void DefaultFrame::moveLeftHandle(const QPointF& P2) {
    QPointF P1 = mapToScene(rect().width(), rect().height());    // fix point

    QTransform t;
    t.rotate(-rotation());
    t.translate(-P1.x(), -P1.y());

    QPointF P2i = t.map(P2);
    QSizeF newSize = QSizeF(-P2i.x(), rect().height()).expandedTo(QSizeF(10, 10));

    QTransform t_;
    t_.translate(P1.x(), P1.y());
    t_.rotate(rotation());
    QPointF pos1 = t_.map(QPointF(-newSize.width(), -newSize.height()));

    activateGeometry(newSize, pos1);
}


QPointF DefaultFrame::getHandleOffset(AbstractEditHandle editHandle, const QPointF& scenePos) {
    QPointF ref = scenePos;

    switch(editHandle) {
        case DefaultFrame::TopLeftHandle :
            ref = mapToScene(topLeft);
            break;

        case DefaultFrame::TopHandle :
            ref = mapToScene(top);
            break;

        case DefaultFrame::TopRightHandle :
            ref = mapToScene(topRight);
            break;

        case DefaultFrame::LeftHandle :
            ref = mapToScene(left);
            break;

        case DefaultFrame::RotationHandle :
            break;

        case DefaultFrame::RightHandle :
            ref = mapToScene(right);
            break;

        case DefaultFrame::BottomLeftHandle :
            ref = mapToScene(bottomLeft);
            break;

        case DefaultFrame::BottomHandle :
            ref = mapToScene(bottom);
            break;

        case DefaultFrame::BottomRightHandle :
            ref = mapToScene(bottomRight);
            break;

        case DefaultFrame::MoveHandle:
            ref = pos();
            break;
    }

    return ref - scenePos;
}


void DefaultFrame::moveHandle(AbstractEditHandle editHandle, const QPointF& scenePos) {
    QPointF newPos;
    QSizeF newSize;

    switch(editHandle) {
        case DefaultFrame::TopLeftHandle :
                moveTopLeftHandle(scenePos);
                break;

        case DefaultFrame::TopHandle :
                moveTopHandle(scenePos);
                break;

        case DefaultFrame::TopRightHandle :
                moveTopRightHandle(scenePos);
                break;

        case DefaultFrame::LeftHandle :
                moveLeftHandle(scenePos);
                break;

        case DefaultFrame::BottomLeftHandle :
                moveBottomLeftHandle(scenePos);
                break;

        case DefaultFrame::RotationHandle :
                moveRotationHandle(scenePos);
                break;

        case DefaultFrame::RightHandle :
                moveRightHandle(scenePos);
                break;

        case DefaultFrame::BottomHandle :
                moveBottomHandle(scenePos);
                break;

        case DefaultFrame::BottomRightHandle :
                moveBottomRightHandle(scenePos);
                break;

        case DefaultFrame::MoveHandle:
                setPos(scenePos.x(), scenePos.y());
                break;
    }

}


void DefaultFrame::setCursor(GraphicsSheet* theView, AbstractEditHandle handle) {

    switch(handle) {
        case DefaultFrame::TopLeftHandle :
                theView->setCursor(Qt::SizeFDiagCursor);
                break;

        case DefaultFrame::TopHandle :
                theView->setCursor(Qt::SizeVerCursor);
                break;

        case DefaultFrame::TopRightHandle :
                theView->setCursor(Qt::SizeBDiagCursor);
                break;

        case DefaultFrame::LeftHandle :
                theView->setCursor(Qt::SizeHorCursor);
                break;

        case DefaultFrame::RotationHandle :
                theView->setCursor(Qt::PointingHandCursor);  // TODO: Create rotation cursor
                break;

        case DefaultFrame::RightHandle :
                theView->setCursor(Qt::SizeHorCursor);
                break;

        case DefaultFrame::BottomLeftHandle :
                theView->setCursor(Qt::SizeBDiagCursor);
                break;

        case DefaultFrame::BottomHandle :
                theView->setCursor(Qt::SizeVerCursor);
                break;

        case DefaultFrame::BottomRightHandle :
                theView->setCursor(Qt::SizeFDiagCursor);
                break;

        case DefaultFrame::MoveHandle :
                theView->setCursor(Qt::OpenHandCursor);
                break;

        default :
                theView->setCursor(Qt::ArrowCursor);
                break;
    }
}


QPointF DefaultFrame::getNearestEdge(GraphicsSheet* view, const QPointF& scenePos) {
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
}


void DefaultFrame::paint ( QPainter * painter, const QStyleOptionGraphicsItem * style, QWidget *widget) {
    // reset the selected state when painting - this is a workaround
    // to avoid that QGraphicsDefaultFrame::paint draws the selection rectangle
    QStyleOptionGraphicsItem option2 = *style;
    option2.state = 0;
    QGraphicsRectItem::paint(painter, &option2, widget);
}
