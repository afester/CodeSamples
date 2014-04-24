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

    draggersCalculated = false;
    setAcceptHoverEvents(true); // TODO: Only necessary when the item is selected!
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemClipsToShape, true);
}


EditableItem::EditableItem(const QRectF & rect, QGraphicsItem * parent) :
    QGraphicsRectItem(0, 0, rect.width(), rect.height(), parent){

    setPos(rect.x(), rect.y());

    draggersCalculated = false;
    setAcceptHoverEvents(true); // TODO: Only necessary when the item is selected!
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemClipsToShape, true);
}


void EditableItem::invalidateDraggers() {
    draggersCalculated = false;
}

void EditableItem::calculateDraggers(GraphicsSheet* view) {
    // Initialize draggers
    const qreal width = rect().width();
    const qreal height = rect().height();
    const qreal xhalf = width / 2;
    const qreal yhalf = height / 2;

    // view->transform()->
    const int size = 3;

    rotationHandle = QRectF(xhalf - size/2, 5,        size, size);
    topLeft = QRectF(0, 0,                            size, size);
    topRight = QRectF(width  - size, 0,               size, size);
    bottomLeft = QRectF(0, height - size,             size, size);
    bottomRight = QRectF(width - size, height - size, size, size);
    top = QRectF(xhalf - size/2, 0,                   size, size);
    bottom = QRectF(xhalf - size/2, height - size,    size, size);
    left = QRectF(0, yhalf - size/2,                  size, size);
    right = QRectF(width - size, yhalf - size/2,      size, size);
}


EditableItem::EditHandle EditableItem::getEditHandle(GraphicsSheet* view, const QPointF& pos, EditHandles enabledHandles) {
    if (!draggersCalculated) {
        calculateDraggers(view);
        draggersCalculated = true;
    }

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

#if 0
void EditableItem::paintSelectedBorder(GraphicsSheet* view, QPainter * painter) {
    if (isSelected()) {
    	// Paint the selection border
    	painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(Qt::black, 0, Qt::DashLine));
        painter->drawRect(rect());
    }
}
#endif

void EditableItem::paintHandles(GraphicsSheet* view, QPainter * painter, EditHandles enabledHandles) {
    if (!draggersCalculated) {
        calculateDraggers(view);
        draggersCalculated = true;
    }

    if (isSelected()) {
        // Paint the handles
		painter->setBrush(QColor(173, 210, 204));
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

	GraphicsSheet* requestingView = dynamic_cast<GraphicsSheet*>(widget ? widget->parent() : 0);
	if (requestingView) {
		Interactor* inter = requestingView->getInteractor();
		if (inter) {
			inter->paintDecorations(this, painter);
		}
	}
}
