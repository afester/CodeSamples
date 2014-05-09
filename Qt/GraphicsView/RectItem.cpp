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


void RectItem::calculateDraggers(GraphicsSheet* view) {
    const qreal handleSize = 8.0;   // TODO: read from view property

    const qreal hsize = handleSize / view->transform().m11();
    const qreal vsize = handleSize / view->transform().m22();

    // Initialize draggers

    const qreal width = rect().width();
    const qreal height = rect().height();
    const qreal xhalf = width / 2;
    const qreal yhalf = height / 2;
    const qreal hsizeHalf = hsize / 2;
    const qreal vsizeHalf = hsize /2;

    rotationHandle = QRectF(xhalf - hsizeHalf,  vsize,              hsize, vsize);
    topLeft =        QRectF(-hsizeHalf,         -vsizeHalf,         hsize, vsize);
    topRight =       QRectF(width - hsizeHalf,  -vsizeHalf,         hsize, vsize);
    bottomLeft =     QRectF(-hsize/2,           height - vsizeHalf, hsize, vsize);
    bottomRight =    QRectF(width - hsizeHalf,  height - vsizeHalf, hsize, vsize);
    top =            QRectF(xhalf - hsizeHalf,  -vsizeHalf,         hsize, vsize);
    bottom =         QRectF(xhalf - hsizeHalf,  height - vsizeHalf, hsize, vsize);
    left =           QRectF(-hsizeHalf,         yhalf - vsizeHalf,  hsize, vsize);
    right =          QRectF(width - hsizeHalf,  yhalf - vsizeHalf,  hsize, vsize);
}


QRectF RectItem::boundingRect() const {
    QRectF result = QGraphicsRectItem::boundingRect();
    return QRectF(result.x() - 2, result.y() - 2, result.width() + 4, result.height() + 4);
}


/*EditableItem::EditHandle*/ unsigned int RectItem::getEditHandle(GraphicsSheet* view, const QPointF& scenePos, unsigned int /*EditHandles */ enabledHandles) {
    calculateDraggers(view);


    QPointF pos = mapFromScene(scenePos);
    if (isSelected()) {
        if ( (enabledHandles & RotationHandleMask) && rotationHandle.contains(pos.toPoint())) {
            return RotationHandle;
        } else if ( (enabledHandles & TopLeftHandleMask) && topLeft.contains(pos.toPoint())) {
        	return TopLeftHandle;
        } else if ( (enabledHandles & TopHandleMask) && top.contains(pos.toPoint())) {
        	return TopHandle;
        } else if ( (enabledHandles & TopRightHandleMask) && topRight.contains(pos.toPoint())) {
        	return TopRightHandle;
        } else if ( (enabledHandles & RightHandleMask) && right.contains(pos.toPoint())) {
        	return RightHandle;
        } else if ( (enabledHandles & BottomRightHandleMask) && bottomRight.contains(pos.toPoint())) {
        	return BottomRightHandle;
        } else if ( (enabledHandles & BottomHandleMask) && bottom.contains(pos.toPoint())) {
        	return BottomHandle;
        } else if ( (enabledHandles & BottomLeftHandleMask) && bottomLeft.contains(pos.toPoint())) {
            return BottomLeftHandle;
        } else if ( (enabledHandles & LeftHandleMask) && left.contains(pos.toPoint())) {
        	return LeftHandle;
        } else if ( (enabledHandles & MoveHandleMask) && rect().contains(pos.toPoint())) {
                return MoveHandle;
        }
    }

    return NoHandle;
}


static QString formatFloat(qreal number, int dec) {
    QString result;
    QTextStream format(&result);
    format << fixed << qSetRealNumberPrecision(dec) << number;
    return result;
}


void RectItem::paintSelectedBorder(GraphicsSheet* view, QPainter * painter) {
    if (isSelected()) {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(Qt::green, 0, Qt::DashLine));

        painter->drawRect(QRectF(0, 0, rect().width(), rect().height()));
    }
}


void RectItem::paintHandles(GraphicsSheet* view, QPainter * painter, unsigned int /*EditHandles*/ enabledHandles) {
    if (isSelected()) {
        calculateDraggers(view);

        // Paint the handles
		painter->setBrush(Qt::green);
		painter->setPen(QPen(Qt::black, 0, Qt::SolidLine));	// Qt5: need to explicitly set width to 0, even though the docs claim other

		if (enabledHandles & RotationHandleMask)
			painter->drawEllipse(rotationHandle);
		if (enabledHandles & TopLeftHandleMask)
			painter->drawRect(topLeft);
		if (enabledHandles & TopHandleMask)
			painter->drawRect(top);
		if (enabledHandles & TopRightHandleMask)
			painter->drawRect(topRight);
		if (enabledHandles & RightHandleMask)
			painter->drawRect(right);
		if (enabledHandles & BottomRightHandleMask)
			painter->drawRect(bottomRight);
		if (enabledHandles & BottomHandleMask)
			painter->drawRect(bottom);
		if (enabledHandles & BottomLeftHandleMask)
			painter->drawRect(bottomLeft);
		if (enabledHandles & LeftHandleMask)
			painter->drawRect(left);
    }
}


void RectItem::moveTopLeftHandle(const QPointF& scenePos) {
    // P1 is the new upper left corner (in scene coordinates)
    QPointF P1 = scenePos;

    // P2 is the fix point (in scene coordinates)
    QPointF P2 = mapToScene(rect().width(), rect().height());

    // transform coordinate system so that its center is at P1
    // we need to transform back (from global coordinates into local coordinates),
    // so we need to do the inverse operations (rotate -angle => translate -P1)
    QTransform t;
    t.rotate(-rotation());
    t.translate(-P1.x(), -P1.y());

    // Map the fix point into the transformed coordinate system -
    // this will be the new width and height
    QPointF P2i = t.map(P2);
    QSizeF newSize = QSizeF(P2i.x(), P2i.y());

    if (newSize.width() < 10) {
        newSize.setWidth(10);
    }
    if (newSize.height() < 10) {
        newSize.setHeight(10);
    }

    // calculate new center point - this will become the new transformation center point
    QPointF P3(newSize.width()/2, newSize.height()/2);

    // transform P1 to the new item position
    QTransform t2;
    t2.translate(P1.x(), P1.y());
    t2.rotate(rotation());
    t2.translate(P3.x(), P3.y());
    t2.rotate(-rotation());
    t2.translate(-P3.x(), -P3.y());
    t2.translate(-P1.x(), -P1.y());
    QPointF newPos = t2.map(P1);

    // activate new geometry
    setPos(newPos);
    setRect(QRectF(0, 0, newSize.width(), newSize.height()));
    setTransformOriginPoint(P3);

    //positionIndicator = QPoint(newPos.x(), newPos.y());
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


void RectItem::moveRightHandle(const QPointF& scenePos) {
    QPointF posInItem = mapFromScene(scenePos);
    QSizeF newSize = QSizeF(posInItem.x(), rect().height());
    if (newSize.width() < 10) {
        newSize.setWidth(10);
    }

    // activate new geometry
    setRect(QRectF(0, 0, newSize.width(), newSize.height()));
}


void RectItem::moveBottomHandle(const QPointF& scenePos) {
    QPointF posInItem = mapFromScene(scenePos);
    QSizeF newSize = QSizeF(rect().width(), posInItem.y());
    if (newSize.height() < 10) {
        newSize.setHeight(10);
    }

    // activate new geometry
    setRect(QRectF(0, 0, newSize.width(), newSize.height()));
}

void RectItem::moveBottomRightHandle(const QPointF& scenePos) {
    QPointF posInItem = mapFromScene(scenePos);
    QSizeF newSize = QSizeF(posInItem.x(), posInItem.y());
    if (newSize.width() < 10) {
        newSize.setWidth(10);
    }
    if (newSize.height() < 10) {
        newSize.setHeight(10);
    }

    // activate new geometry
    setRect(QRectF(0, 0, newSize.width(), newSize.height()));
}



void RectItem::moveTopHandle(const QPointF& scenePos) {
    // P1 is the new upper left corner (in scene coordinates)
    QPointF P1 = scenePos;

    // P2 is the fix point (in scene coordinates)
    QPointF P2 = mapToScene(rect().width(), rect().height());

    // transform coordinate system so that its center is at P1
    // we need to transform back (from global coordinates into local coordinates),
    // so we need to do the inverse operations (rotate -angle => translate -P1)
    QTransform t;
    t.rotate(-rotation());
    t.translate(-P1.x(), -P1.y());

    // Map the fix point into the transformed coordinate system -
    // this will be the new height
    QPointF P2i = t.map(P2);
    QSizeF newSize = QSizeF(rect().width(), P2i.y());
    if (newSize.height() < 10) {
        newSize.setHeight(10);
    }

    // calculate new center point - this will become the new transformation center point
    QPointF P3(newSize.width()/2, newSize.height()/2);

    QTransform t2;
    t2.translate(P2.x(), P2.y());
    t2.rotate(rotation());
    t2.translate(-newSize.width(), -newSize.height());
    QPointF pos_ = t2.map(QPointF(0, 0));

    // transform P1 to the new item position
    QTransform t3;
    t3.translate(P1.x(), P1.y());
    t3.rotate(rotation());
    t3.translate(P3.x(), P3.y());
    t3.rotate(-rotation());
    t3.translate(-P3.x(), -P3.y());
    t3.translate(-P1.x(), -P1.y());
    QPointF newPos = t3.map(pos_);

    // activate new geometry
    setPos(newPos);
    setRect(QRectF(0, 0, newSize.width(), newSize.height()));
    setTransformOriginPoint(P3);
}


void RectItem::moveLeftHandle(const QPointF& scenePos) {
    // P1 is the new upper right corner (in scene coordinates)
    QPointF P1 = scenePos;

    // P2 is the fix point (in scene coordinates)
    QPointF P2 = mapToScene(rect().width(), rect().height());

    // transform coordinate system so that its center is at P1
    // we need to transform back (from global coordinates into local coordinates),
    // so we need to do the inverse operations (rotate -angle => translate -P1)
    QTransform t;
    t.rotate(-rotation());
    t.translate(-P1.x(), -P1.y());

    // Map the fix point into the transformed coordinate system -
    // this will be the new height
    QPointF P2i = t.map(P2);
    QSizeF newSize = QSizeF(P2i.x(), rect().height());
    if (newSize.width() < 10) {
        newSize.setWidth(10);
    }

    // calculate new center point - this will become the new transformation center point
    QPointF P3(newSize.width()/2, newSize.height()/2);

    QTransform t2;
    t2.translate(P2.x(), P2.y());
    t2.rotate(rotation());
    t2.translate(-newSize.width(), -newSize.height());
    QPointF pos_ = t2.map(QPointF(0, 0));

    // transform P1 to the new item position
    QTransform t3;
    t3.translate(P1.x(), P1.y());
    t3.rotate(rotation());
    t3.translate(P3.x(), P3.y());
    t3.rotate(-rotation());
    t3.translate(-P3.x(), -P3.y());
    t3.translate(-P1.x(), -P1.y());
    QPointF newPos = t3.map(pos_);

    // activate new geometry
    setPos(newPos);
    setRect(QRectF(0, 0, newSize.width(), newSize.height()));
    setTransformOriginPoint(P3);
}


void RectItem::moveTopRightHandle(const QPointF& scenePos) {
#if 0
#endif
}

void RectItem::moveBottomLeftHandle(const QPointF& scenePos) {
#if 0
        case RectItem::BottomLeftHandle :
                newSize = QSizeF(theFrame->rect().width() - posInItem.x(),
                                 posInItem.y()); //  - theFrame->y());
                if (newSize.width() < 10) {
                    newSize.setWidth(10);
                }
                if (newSize.height() < 10) {
                    newSize.setHeight(10);
                }
                //newPos = QPointF(theFrame->x() + theFrame->rect().width() - newSize.width() , theFrame->y());
                newPos = QPointF(theFrame->x() + posInItem.x(), theFrame->y());

                positionIndicator = QPoint(newPos.x(), newPos.y() + newSize.height());
                break;
#endif
}

QSizeF RectItem::getHandleOffset(unsigned int editHandle, const QPointF& scenePos) {
    switch(editHandle) {
        case RectItem::TopLeftHandle :
                return QSizeF(0, 0); // theFrame->x() - scenePos.x(), theFrame->y() - scenePos.y());
                //positionIndicator = QPoint(theFrame->x(), theFrame->y());

        case RectItem::TopHandle :
                return QSizeF(0, 0); // QSizeF(0, theFrame->y() - scenePos.y());
                //positionIndicator = QPoint(-1, theFrame->y());

        case RectItem::TopRightHandle :
                return QSizeF(0, 0); // QSizeF(theFrame->x() + theFrame->rect().width() - scenePos.x(), theFrame->y() - scenePos.y());
               // positionIndicator = QPoint(theFrame->x() + theFrame->rect().width(), theFrame->y());

        case RectItem::LeftHandle :
                return QSizeF(0, 0); // QSizeF(theFrame->x() - scenePos.x(), 0);
                //positionIndicator = QPoint(theFrame->x(), -1);

        case RectItem::RotationHandle :
                return QSizeF(0, 0);

        case RectItem::RightHandle :
                return QSizeF(0, 0); // QSizeF(theFrame->x() + theFrame->rect().width() - scenePos.x(), 0);
                //positionIndicator = QPoint(theFrame->x() + theFrame->rect().width(), -1);

        case RectItem::BottomLeftHandle :
                return QSizeF(0, 0); // QSizeF(theFrame->x() - scenePos.x(), theFrame->y() + theFrame->rect().height() - scenePos.y());
                //positionIndicator = QPoint(theFrame->x(), theFrame->y() + theFrame->rect().height());

        case RectItem::BottomHandle :
                return QSizeF(0, 0); // QSizeF(0, theFrame->y() + theFrame->rect().height() - scenePos.y());
               // positionIndicator = QPoint(-1, theFrame->y() + theFrame->rect().height());

        case RectItem::BottomRightHandle :
                return QSizeF(0, 0); // QSizeF(theFrame->x() + theFrame->rect().width() - scenePos.x(),
                               //theFrame->y() + theFrame->rect().height() - scenePos.y());
               // positionIndicator = QPoint(theFrame->x() + theFrame->rect().width(),
            //                               theFrame->y() + theFrame->rect().height());

        case RectItem::MoveHandle:
                return QSize(x() - scenePos.x(), y() - scenePos.y());
                break;
    }
}

void RectItem::moveHandle(unsigned int /*EditHandle*/ editHandle, const QPointF& scenePos) {
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


void RectItem::setCursor(GraphicsSheet* theView, unsigned int /*EditHandle*/ handle) {

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
