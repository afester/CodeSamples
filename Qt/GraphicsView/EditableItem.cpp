#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QCursor>
#include <QRectF>
#include <QPointF>
#include <QAction>
#include <QTextStream>

#include <math.h>


#include "EditableItem.h"
// #include "KollageGraphicsScene.h"
#include "GraphicsSheet.h"
// #include "Log.h"
#include "Interactor.h"

#if 0
void ItemVisitor::visitSelectedItems(QGraphicsScene* scene) {
    QList<QGraphicsItem*> selectedItems = scene->selectedItems();
    QGraphicsItem* item;
    foreach(item, selectedItems) {
        EditableItem* theItem = dynamic_cast<EditableItem*>(item);
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

EditableItem::EditableItem(const QPointF& pos, QGraphicsItem * parent) :
    QGraphicsRectItem(0, 0, 10, 10, parent) {

    setPos(pos.x(), pos.y());

    setAcceptHoverEvents(true); // TODO: Only necessary when the item is selected!
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemClipsToShape, true);
}


EditableItem::EditableItem(const QRectF & rect, QGraphicsItem * parent) :
    QGraphicsRectItem(0, 0, rect.width(), rect.height(), parent){

    setPos(rect.x(), rect.y());

    setAcceptHoverEvents(true); // TODO: Only necessary when the item is selected!
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemClipsToShape, true);
}


void EditableItem::calculateDraggers(GraphicsSheet* view) {
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


QRectF EditableItem::boundingRect() const {
    QRectF result = QGraphicsRectItem::boundingRect();

    return QRectF(result.x() - 2, result.y() - 2, result.width() + 4, result.height() + 4);
}


EditableItem::EditHandle EditableItem::getEditHandle(GraphicsSheet* view, const QPointF& pos, EditHandles enabledHandles) {
    calculateDraggers(view);

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


void EditableItem::paintSelectedBorder(GraphicsSheet* view, QPainter * painter) {
    if (isSelected()) {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(Qt::green, 0, Qt::DashLine));

        painter->drawRect(QRectF(0, 0, rect().width(), rect().height()));
    }
}


void EditableItem::paintHandles(GraphicsSheet* view, QPainter * painter, EditHandles enabledHandles) {
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


void EditableItem::moveTopLeftHandle(const QPointF& scenePos) {
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


void EditableItem::moveRotationHandle(const QPointF& scenePos) {
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


void EditableItem::moveRightHandle(const QPointF& scenePos) {
    QPointF posInItem = mapFromScene(scenePos);
    QSizeF newSize = QSizeF(posInItem.x(), rect().height());
    if (newSize.width() < 10) {
        newSize.setWidth(10);
    }

    // activate new geometry
    setRect(QRectF(0, 0, newSize.width(), newSize.height()));
}


void EditableItem::moveBottomHandle(const QPointF& scenePos) {
    QPointF posInItem = mapFromScene(scenePos);
    QSizeF newSize = QSizeF(rect().width(), posInItem.y());
    if (newSize.height() < 10) {
        newSize.setHeight(10);
    }

    // activate new geometry
    setRect(QRectF(0, 0, newSize.width(), newSize.height()));
}

void EditableItem::moveBottomRightHandle(const QPointF& scenePos) {
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



void EditableItem::moveTopHandle(const QPointF& scenePos) {
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


void EditableItem::moveLeftHandle(const QPointF& scenePos) {
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


void EditableItem::moveTopRightHandle(const QPointF& scenePos) {
#if 0
#endif
}

void EditableItem::moveBottomLeftHandle(const QPointF& scenePos) {
#if 0
        case EditableItem::BottomLeftHandle :
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


void EditableItem::moveHandle(EditHandle editHandle, const QPointF& scenePos) {
    QPointF newPos;
    QSizeF newSize;

    switch(editHandle) {
        case EditableItem::TopLeftHandle :
                moveTopLeftHandle(scenePos);
                break;

        case EditableItem::TopHandle :
                moveTopHandle(scenePos);
                break;

        case EditableItem::TopRightHandle :
                moveTopRightHandle(scenePos);
                break;

        case EditableItem::LeftHandle :
                moveLeftHandle(scenePos);
                break;

        case EditableItem::BottomLeftHandle :
                moveBottomLeftHandle(scenePos);
                break;

        case EditableItem::RotationHandle :
                moveRotationHandle(scenePos);
                break;

        case EditableItem::RightHandle :
                moveRightHandle(scenePos);
                break;

        case EditableItem::BottomHandle :
                moveBottomHandle(scenePos);
                break;

        case EditableItem::BottomRightHandle :
                moveBottomRightHandle(scenePos);
                break;

        case EditableItem::MoveHandle:
                setPos(scenePos.x(), scenePos.y());
                break;
    }

}


void EditableItem::setCursor(GraphicsSheet* theView, EditHandle handle) {

    switch(handle) {
        case EditableItem::TopLeftHandle :
                theView->setCursor(Qt::SizeFDiagCursor);
                break;

        case EditableItem::TopHandle :
                theView->setCursor(Qt::SizeVerCursor);
                break;

        case EditableItem::TopRightHandle :
                theView->setCursor(Qt::SizeBDiagCursor);
                break;

        case EditableItem::LeftHandle :
                theView->setCursor(Qt::SizeHorCursor);
                break;

        case EditableItem::RotationHandle :
                theView->setCursor(Qt::PointingHandCursor);  // TODO: Create rotation cursor
                break;

        case EditableItem::RightHandle :
                theView->setCursor(Qt::SizeHorCursor);
                break;

        case EditableItem::BottomLeftHandle :
                theView->setCursor(Qt::SizeBDiagCursor);
                break;

        case EditableItem::BottomHandle :
                theView->setCursor(Qt::SizeVerCursor);
                break;

        case EditableItem::BottomRightHandle :
                theView->setCursor(Qt::SizeFDiagCursor);
                break;

        case EditableItem::MoveHandle :
                theView->setCursor(Qt::OpenHandCursor);
                break;

        default :
                theView->setCursor(Qt::ArrowCursor);
                break;
    }
}


void EditableItem::paintCoordinates(GraphicsSheet* view, QPainter* painter) {
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


void EditableItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * style, QWidget *widget) {
    QGraphicsRectItem::paint(painter, style, widget);
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
