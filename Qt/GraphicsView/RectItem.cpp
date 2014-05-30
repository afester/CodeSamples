#include <QDebug>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QCursor>
#include <QRectF>
#include <QStyleOptionGraphicsItem>
#include <QPointF>
#include <QAction>
#include <QTextStream>

#include <math.h>


#include "RectItem.h"
// #include "KollageGraphicsScene.h"
#include "GraphicsSheet.h"
// #include "Log.h"
#include "Interactor.h"

#if 0
void ItemVisitor::visitSelectedItems(QGraphicsScene* scene) {
    QList<QGraphicsItem*> selectedItems = scene->selectedItems();
    QGraphicsItem* item;
    foreach(item, selectedItems) {
        RectItem* theItem = dynamic_cast<RectItem*>(item);
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

RectItem::RectItem(const QPointF& pos, QGraphicsItem * parent) :
    QGraphicsRectItem(0, 0, 10, 10, parent) {

    setPos(pos.x(), pos.y());

    setAcceptHoverEvents(true); // TODO: Only necessary when the item is selected!
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemClipsToShape, true);
}


RectItem::RectItem(const QRectF & rect, QGraphicsItem * parent) :
    QGraphicsRectItem(0, 0, rect.width(), rect.height(), parent){

    setPos(rect.x(), rect.y());

    setAcceptHoverEvents(true); // TODO: Only necessary when the item is selected!
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemClipsToShape, true);
}


void RectItem::calculateHandles(GraphicsSheet* view) {
    const qreal handleSize = 8.0;   // TODO: read from view property

    hsize = handleSize / view->transform().m11();
    vsize = handleSize / view->transform().m22();

    const qreal width = rect().width();
    const qreal height = rect().height();
    const qreal xhalf = width / 2;
    const qreal yhalf = height / 2;

    rotationHandle = QPointF(xhalf, 2*vsize);
    topLeft        = QPointF(0, 0);
    topRight       = QPointF(width, 0);
    bottomLeft     = QPointF(0, height);
    bottomRight    = QPointF(width, height);
    top            = QPointF(xhalf, 0);
    bottom         = QPointF(xhalf, height);
    left           = QPointF(0, yhalf);
    right          = QPointF(width, yhalf);
}


QRectF RectItem::boundingRect() const {
    QRectF result = QGraphicsRectItem::boundingRect();
    return QRectF(result.x() - 2, result.y() - 2, result.width() + 4, result.height() + 4);
}


AbstractEditHandle RectItem::getEditHandle(GraphicsSheet* view, const QPointF& scenePos, unsigned int /*EditHandles */ enabledHandles) {
    calculateHandles(view);

    QPointF pos = mapFromScene(scenePos);
    QRectF area = QRectF(pos.x() - hsize/2, pos.y() - vsize/2, hsize, vsize);
    if (isSelected()) {

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

#if 0
static QString formatFloat(qreal number, int dec) {
    QString result;
    QTextStream format(&result);
    format << fixed << qSetRealNumberPrecision(dec) << number;
    return result;
}
#endif

void RectItem::paintSelectedBorder(GraphicsSheet* view, QPainter * painter) {
    Q_UNUSED(view);

    if (isSelected()) {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(Qt::green, 0, Qt::DashLine));

        painter->drawRect(QRectF(0, 0, rect().width(), rect().height()));
    }
}


void RectItem::paintHandles(GraphicsSheet* view, QPainter * painter, AbstractEditHandle enabledHandles) {
    if (isSelected()) {
        calculateHandles(view);

        // Paint the handles
		painter->setBrush(Qt::green);
		painter->setPen(QPen(Qt::black, 0, Qt::SolidLine));	// Qt5: need to explicitly set width to 0, even though the docs claim other

		if (enabledHandles & RotationHandleMask)
			painter->drawEllipse(rotationHandle, hsize/2, vsize/2);
		if (enabledHandles & TopLeftHandleMask)
			painter->drawRect(QRectF(topLeft.x() - hsize/2, topLeft.y() - vsize/2, hsize, vsize));
		if (enabledHandles & TopHandleMask)
		    painter->drawRect(QRectF(top.x() - hsize/2, top.y() - vsize/2, hsize, vsize));
		if (enabledHandles & TopRightHandleMask)
            painter->drawRect(QRectF(topRight.x() - hsize/2, topRight.y() - vsize/2, hsize, vsize));
		if (enabledHandles & RightHandleMask)
		    painter->drawRect(QRectF(right.x() - hsize/2, right.y() - vsize/2, hsize, vsize));
		if (enabledHandles & BottomRightHandleMask)
            painter->drawRect(QRectF(bottomRight.x() - hsize/2, bottomRight.y() - vsize/2, hsize, vsize));
		if (enabledHandles & BottomHandleMask)
            painter->drawRect(QRectF(bottom.x() - hsize/2, bottom.y() - vsize/2, hsize, vsize));
		if (enabledHandles & BottomLeftHandleMask)
            painter->drawRect(QRectF(bottomLeft.x() - hsize/2, bottomLeft.y() - vsize/2, hsize, vsize));
		if (enabledHandles & LeftHandleMask)
            painter->drawRect(QRectF(left.x() - hsize/2, left.y() - vsize/2, hsize, vsize));
    }
}


void RectItem::moveRotationHandle(const QPointF& scenePos) {
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


void RectItem::activateGeometry(const QSizeF& newSize, const QPointF& pos1) {
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


void RectItem::moveTopLeftHandle(const QPointF& P2) {

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


void RectItem::moveBottomRightHandle(const QPointF& P2) {

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


void RectItem::moveTopRightHandle(const QPointF& P2) {

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


void RectItem::moveBottomLeftHandle(const QPointF& P2) {

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


void RectItem::moveRightHandle(const QPointF& scenePos) {

    QPointF pos1 = mapToScene(0, 0);    // fix point

    QPointF posInItem = mapFromScene(scenePos);
    QSizeF newSize = QSizeF(posInItem.x(), rect().height()).expandedTo(QSizeF(10, 10));

    activateGeometry(newSize, pos1);
}


void RectItem::moveBottomHandle(const QPointF& scenePos) {

    QPointF pos1 = mapToScene(0, 0);    // fix point

    QPointF posInItem = mapFromScene(scenePos);
    QSizeF newSize = QSizeF(rect().width(), posInItem.y()).expandedTo(QSizeF(10, 10));

    activateGeometry(newSize, pos1);
}


void RectItem::moveTopHandle(const QPointF& P2) {

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


void RectItem::moveLeftHandle(const QPointF& P2) {
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


QPointF RectItem::getHandleOffset(AbstractEditHandle editHandle, const QPointF& scenePos) {
    QPointF ref = scenePos;

    switch(editHandle) {
        case RectItem::TopLeftHandle :
            ref = mapToScene(topLeft);
            break;

        case RectItem::TopHandle :
            ref = mapToScene(top);
            break;

        case RectItem::TopRightHandle :
            ref = mapToScene(topRight);
            break;

        case RectItem::LeftHandle :
            ref = mapToScene(left);
            break;

        case RectItem::RotationHandle :
            break;

        case RectItem::RightHandle :
            ref = mapToScene(right);
            break;

        case RectItem::BottomLeftHandle :
            ref = mapToScene(bottomLeft);
            break;

        case RectItem::BottomHandle :
            ref = mapToScene(bottom);
            break;

        case RectItem::BottomRightHandle :
            ref = mapToScene(bottomRight);
            break;

        case RectItem::MoveHandle:
            ref = pos();
            break;
    }

    return ref - scenePos;
}


void RectItem::moveHandle(AbstractEditHandle editHandle, const QPointF& scenePos) {
    QPointF newPos;
    QSizeF newSize;

    switch(editHandle) {
        case RectItem::TopLeftHandle :
                moveTopLeftHandle(scenePos);
                break;

        case RectItem::TopHandle :
                moveTopHandle(scenePos);
                break;

        case RectItem::TopRightHandle :
                moveTopRightHandle(scenePos);
                break;

        case RectItem::LeftHandle :
                moveLeftHandle(scenePos);
                break;

        case RectItem::BottomLeftHandle :
                moveBottomLeftHandle(scenePos);
                break;

        case RectItem::RotationHandle :
                moveRotationHandle(scenePos);
                break;

        case RectItem::RightHandle :
                moveRightHandle(scenePos);
                break;

        case RectItem::BottomHandle :
                moveBottomHandle(scenePos);
                break;

        case RectItem::BottomRightHandle :
                moveBottomRightHandle(scenePos);
                break;

        case RectItem::MoveHandle:
                setPos(scenePos.x(), scenePos.y());
                break;
    }

}


void RectItem::setCursor(GraphicsSheet* theView, AbstractEditHandle handle) {

    switch(handle) {
        case RectItem::TopLeftHandle :
                theView->setCursor(Qt::SizeFDiagCursor);
                break;

        case RectItem::TopHandle :
                theView->setCursor(Qt::SizeVerCursor);
                break;

        case RectItem::TopRightHandle :
                theView->setCursor(Qt::SizeBDiagCursor);
                break;

        case RectItem::LeftHandle :
                theView->setCursor(Qt::SizeHorCursor);
                break;

        case RectItem::RotationHandle :
                theView->setCursor(Qt::PointingHandCursor);  // TODO: Create rotation cursor
                break;

        case RectItem::RightHandle :
                theView->setCursor(Qt::SizeHorCursor);
                break;

        case RectItem::BottomLeftHandle :
                theView->setCursor(Qt::SizeBDiagCursor);
                break;

        case RectItem::BottomHandle :
                theView->setCursor(Qt::SizeVerCursor);
                break;

        case RectItem::BottomRightHandle :
                theView->setCursor(Qt::SizeFDiagCursor);
                break;

        case RectItem::MoveHandle :
                theView->setCursor(Qt::OpenHandCursor);
                break;

        default :
                theView->setCursor(Qt::ArrowCursor);
                break;
    }
}

QPointF RectItem::getNearestEdge(GraphicsSheet* view, const QPointF& scenePos) {
    QPointF pos = mapFromScene(scenePos);

    // topLeft, topRight, bottomRight, bottomLeft
    // QPointF p1 = scenePos - topLeft;

    calculateHandles(view);

    qreal min = (pos - topLeft).manhattanLength();
    qreal l2 = (pos - topRight).manhattanLength();
    qreal l3 = (pos - bottomRight).manhattanLength();
    qreal l4 = (pos - bottomLeft).manhattanLength();
    qDebug() << min << ", " << l2<<", "<<l3<<", "<<l4;

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


#if 0
void RectItem::paintCoordinates(GraphicsSheet* view, QPainter* painter) {
    if (isSelected()) {
		// Paint the coordinates
		painter->setFont(QFont("Arial", 2, 1, false));
		QFontMetrics fontMetrics(painter->fontMetrics());

		QString upperLeft = formatFloat(this->pos().x(), 1) + "/" +
							formatFloat(this->pos().y(), 1);
		QSize textSize = fontMetrics.size(0, upperLeft);
		QRectF upperLeftRect = QRectF(3, 3, textSize.width() + 1, textSize.height());

		QString lowerRight =
				formatFloat(this->pos().x() + this->rect().width(), 1) + "/" +
				formatFloat(this->pos().y() + this->rect().height(), 1);
		textSize = fontMetrics.size(0, lowerRight);
		QRectF lowerRightRect =
				QRectF(this->rect().width() - textSize.width() - 3,
					   this->rect().height() - textSize.height() - 3,
					   textSize.width() + 1, textSize.height());

		QString size = formatFloat(this->rect().width(), 1) + " x " +
					   formatFloat(this->rect().height(), 1);
		textSize = fontMetrics.size(0, size);
		QRectF sizeRect =
				QRectF((this->rect().width() - textSize.width()) / 2,
					   (this->rect().height() - textSize.height()) / 2,
						textSize.width() + 1, textSize.height());

		// visualize upper left corner, lower right corner, width and height
		QPointF textPos = upperLeftRect.bottomLeft();
		textPos += QPointF(1.0, -1.0);
		painter->fillRect(upperLeftRect, Qt::lightGray);
		painter->setPen(Qt::white);
		painter->drawRect(upperLeftRect);
		painter->setPen(Qt::black);
		painter->drawText(textPos, upperLeft);

		textPos = lowerRightRect.bottomLeft();
		textPos += QPointF(1.0, -1.0);
		painter->fillRect(lowerRightRect, Qt::lightGray);
		painter->setPen(Qt::white);
		painter->drawRect(lowerRightRect);
		painter->setPen(Qt::black);
		painter->drawText(textPos, lowerRight);

		textPos = sizeRect.bottomLeft();
		textPos += QPointF(1.0, -1.0);
		painter->fillRect(sizeRect, Qt::lightGray);
		painter->setPen(Qt::white);
		painter->drawRect(sizeRect);
		painter->setPen(Qt::black);
		painter->drawText(textPos, size);
    }
}
#endif

void RectItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * style, QWidget *widget) {
    // reset the selected state when painting - this is a workaround
    // to avoid that QGraphicsRectItem::paint draws the selection rectangle
    QStyleOptionGraphicsItem option2 = *style;
    option2.state = 0;
    QGraphicsRectItem::paint(painter, &option2, widget);

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
