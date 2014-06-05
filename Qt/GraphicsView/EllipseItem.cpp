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
#include <QXmlStreamWriter>

#include <math.h>

#include "EllipseItem.h"
#include "GraphicsSheet.h"
#include "Interactor.h"

#if 0
void ItemVisitor::visitSelectedItems(QGraphicsScene* scene) {
    QList<QGraphicsItem*> selectedItems = scene->selectedItems();
    QGraphicsItem* item;
    foreach(item, selectedItems) {
        EllipseItem* theItem = dynamic_cast<EllipseItem*>(item);
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

EllipseItem::EllipseItem() : QGraphicsEllipseItem(0) {
    setAcceptHoverEvents(true); // TODO: Only necessary when the item is selected!
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemClipsToShape, true);
}

EllipseItem::EllipseItem(const QPointF& pos, QGraphicsItem * parent) :
    QGraphicsEllipseItem(-5, -5, 5, 5, parent) {

    setPos(pos.x(), pos.y());

    setAcceptHoverEvents(true); // TODO: Only necessary when the item is selected!
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemClipsToShape, true);
}


EllipseItem::EllipseItem(const QRectF & rect, QGraphicsItem * parent) :
    QGraphicsEllipseItem(-rect.width(), -rect.height(), rect.width(), rect.height(), parent){

    setPos(rect.x() + rect.width()/2, rect.y()+rect.height()/2);

    setAcceptHoverEvents(true); // TODO: Only necessary when the item is selected!
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemClipsToShape, true);
}


QGraphicsItem* EllipseItem::create() {
    return new EllipseItem();
}


void EllipseItem::calculateHandles(GraphicsSheet* view) {
    const qreal width = rect().width();
    const qreal height = rect().height();
    const qreal xhalf = width / 2;
    const qreal yhalf = height / 2;

    rotationHandle = QPointF(0, -yhalf + 2 * view->getHandleSize().height());
    topLeft        = QPointF(-xhalf, -yhalf);
    topRight       = QPointF(xhalf, -yhalf);
    bottomLeft     = QPointF(-xhalf, yhalf);
    bottomRight    = QPointF(xhalf, yhalf);
    top            = QPointF(0, -yhalf);
    bottom         = QPointF(0, yhalf);
    left           = QPointF(-xhalf, 0);
    right          = QPointF(xhalf, 0);
}


QRectF EllipseItem::boundingRect() const {
    QRectF result = QGraphicsEllipseItem::boundingRect();
    return QRectF(result.x() - 2, result.y() - 2, result.width() + 4, result.height() + 4);
}


QPainterPath EllipseItem::shape () const {
    QPainterPath path;

    QRectF r = rect();
    r.adjust(-2,-2,2,2);
    path.addRect(r);

    return path;
}


AbstractEditHandle EllipseItem::getEditHandle(GraphicsSheet* view, const QPointF& scenePos, unsigned int /*EditHandles */ enabledHandles) {
    if (isSelected()) {
        calculateHandles(view);

        QSizeF handleSize = view->getHandleSize();
        QPointF pos = mapFromScene(scenePos);
        QRectF area = QRectF(pos.x() - handleSize.width() / 2, pos.y() - handleSize.height() / 2,
                             handleSize.width(), handleSize.height());

        if ( (enabledHandles & RotationHandleMask) && area.contains(rotationHandle)) {
            return RotationHandle;
        } else if ( (enabledHandles & TopLeftHandleMask) && area.contains(topLeft)) {
        	return TopLeftHandle;
        } else if ( (enabledHandles & TopHandleMask) && area.contains(top)) {
        	return TopHandle;
        } else if ( (enabledHandles & TopRightHandleMask) && area.contains(topRight)) {
        	return TopRightHandle;
        } else if ( (enabledHandles & RightHandleMask) && area.contains(right)) {
        	return RightHandle;
        } else if ( (enabledHandles & BottomRightHandleMask) && area.contains(bottomRight)) {
        	return BottomRightHandle;
        } else if ( (enabledHandles & BottomHandleMask) && area.contains(bottom)) {
        	return BottomHandle;
        } else if ( (enabledHandles & BottomLeftHandleMask) && area.contains(bottomLeft)) {
            return BottomLeftHandle;
        } else if ( (enabledHandles & LeftHandleMask) && area.contains(left)) {
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

void EllipseItem::paintSelectedBorder(GraphicsSheet* view, QPainter * painter) {
    Q_UNUSED(view);

    if (isSelected()) {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(Qt::green, 0, Qt::DashLine));
        painter->drawRect(rect());

        // draw a center cross
        QSizeF handleSize = view->getHandleSize() / 2;
        painter->setPen(QPen(Qt::green, 0, Qt::SolidLine));
        painter->drawLine(QPointF(-handleSize.width(), 0),
                          QPointF(handleSize.width(), 0));
        painter->drawLine(QPointF(0, -handleSize.height()),
                          QPointF(0, handleSize.height()));
    }
}


void EllipseItem::paintHandles(GraphicsSheet* view, QPainter * painter, AbstractEditHandle enabledHandles) {
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


void EllipseItem::moveRotationHandle(const QPointF& scenePos) {
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

    setRotation(angle);
}


void EllipseItem::moveTopLeftHandle(const QPointF& P2) {
    // position in item - note that rotation center is fixed at 0/0 and the
    // position does not change!
    QPointF posInItem = mapFromScene(P2);

    qreal xradius = qMax(-posInItem.x(), 5.0);
    qreal yradius = qMax(-posInItem.y(), 5.0);

    setRect(QRectF(-xradius, -yradius, 2*xradius, 2*yradius));
}


void EllipseItem::moveBottomRightHandle(const QPointF& P2) {

    // position in item - note that rotation center is fixed at 0/0 and the
    // position does not change!
    QPointF posInItem = mapFromScene(P2);

    qreal xradius = qMax(posInItem.x(), 5.0);
    qreal yradius = qMax(posInItem.y(), 5.0);

    setRect(QRectF(-xradius, -yradius, 2*xradius, 2*yradius));
}


void EllipseItem::moveTopRightHandle(const QPointF& P2) {
    // position in item - note that rotation center is fixed at 0/0 and the
    // position does not change!
    QPointF posInItem = mapFromScene(P2);

    qreal xradius = qMax(posInItem.x(), 5.0);
    qreal yradius = qMax(-posInItem.y(), 5.0);

    setRect(QRectF(-xradius, -yradius, 2*xradius, 2*yradius));
}


void EllipseItem::moveBottomLeftHandle(const QPointF& P2) {
    // position in item - note that rotation center is fixed at 0/0 and the
    // position does not change!
    QPointF posInItem = mapFromScene(P2);

    qreal xradius = qMax(-posInItem.x(), 5.0);
    qreal yradius = qMax(posInItem.y(), 5.0);

    setRect(QRectF(-xradius, -yradius, 2*xradius, 2*yradius));
}


void EllipseItem::moveRightHandle(const QPointF& scenePos) {
    // position in item - note that rotation center is fixed at 0/0 and the
    // position does not change!
    QPointF posInItem = mapFromScene(scenePos);

    qreal xradius = qMax(posInItem.x(), 5.0);
    qreal yradius = rect().height()/2; //  qMax(posInItem.y(), 5.0);

    setRect(QRectF(-xradius, -yradius, 2*xradius, 2*yradius));
}


void EllipseItem::moveBottomHandle(const QPointF& scenePos) {
    // position in item - note that rotation center is fixed at 0/0 and the
    // position does not change!
    QPointF posInItem = mapFromScene(scenePos);

    qreal xradius = rect().width()/2; // qMax(posInItem.x(), 5.0);
    qreal yradius = qMax(posInItem.y(), 5.0); // rect().height()/2; //  qMax(posInItem.y(), 5.0);

    setRect(QRectF(-xradius, -yradius, 2*xradius, 2*yradius));
}


void EllipseItem::moveTopHandle(const QPointF& P2) {
    // position in item - note that rotation center is fixed at 0/0 and the
    // position does not change!
    QPointF posInItem = mapFromScene(P2);

    qreal xradius = rect().width()/2; // qMax(posInItem.x(), 5.0);
    qreal yradius = qMax(-posInItem.y(), 5.0); // rect().height()/2; //  qMax(posInItem.y(), 5.0);

    setRect(QRectF(-xradius, -yradius, 2*xradius, 2*yradius));
}


void EllipseItem::moveLeftHandle(const QPointF& P2) {
    // position in item - note that rotation center is fixed at 0/0 and the
    // position does not change!
    QPointF posInItem = mapFromScene(P2);

    qreal xradius = qMax(-posInItem.x(), 5.0);
    qreal yradius = rect().height()/2; //  qMax(posInItem.y(), 5.0);

    setRect(QRectF(-xradius, -yradius, 2*xradius, 2*yradius));
}


QPointF EllipseItem::getHandleOffset(AbstractEditHandle editHandle, const QPointF& scenePos) {
    QPointF ref = scenePos;

    switch(editHandle) {
        case EllipseItem::TopLeftHandle :
            ref = mapToScene(topLeft);
            break;

        case EllipseItem::TopHandle :
            ref = mapToScene(top);
            break;

        case EllipseItem::TopRightHandle :
            ref = mapToScene(topRight);
            break;

        case EllipseItem::LeftHandle :
            ref = mapToScene(left);
            break;

        case EllipseItem::RotationHandle :
            break;

        case EllipseItem::RightHandle :
            ref = mapToScene(right);
            break;

        case EllipseItem::BottomLeftHandle :
            ref = mapToScene(bottomLeft);
            break;

        case EllipseItem::BottomHandle :
            ref = mapToScene(bottom);
            break;

        case EllipseItem::BottomRightHandle :
            ref = mapToScene(bottomRight);
            break;

        case EllipseItem::MoveHandle:
            ref = pos();
            break;
    }

    return ref - scenePos;
}


void EllipseItem::moveHandle(AbstractEditHandle editHandle, const QPointF& scenePos) {
    QPointF newPos;
    QSizeF newSize;

    switch(editHandle) {
        case EllipseItem::TopLeftHandle :
                moveTopLeftHandle(scenePos);
                break;

        case EllipseItem::TopHandle :
                moveTopHandle(scenePos);
                break;

        case EllipseItem::TopRightHandle :
                moveTopRightHandle(scenePos);
                break;

        case EllipseItem::LeftHandle :
                moveLeftHandle(scenePos);
                break;

        case EllipseItem::BottomLeftHandle :
                moveBottomLeftHandle(scenePos);
                break;

        case EllipseItem::RotationHandle :
                moveRotationHandle(scenePos);
                break;

        case EllipseItem::RightHandle :
                moveRightHandle(scenePos);
                break;

        case EllipseItem::BottomHandle :
                moveBottomHandle(scenePos);
                break;

        case EllipseItem::BottomRightHandle :
                moveBottomRightHandle(scenePos);
                break;

        case EllipseItem::MoveHandle:
                setPos(scenePos.x(), scenePos.y());
                break;
    }

}


void EllipseItem::setCursor(GraphicsSheet* theView, AbstractEditHandle handle) {

    switch(handle) {
        case EllipseItem::TopLeftHandle :
                theView->setCursor(Qt::SizeFDiagCursor);
                break;

        case EllipseItem::TopHandle :
                theView->setCursor(Qt::SizeVerCursor);
                break;

        case EllipseItem::TopRightHandle :
                theView->setCursor(Qt::SizeBDiagCursor);
                break;

        case EllipseItem::LeftHandle :
                theView->setCursor(Qt::SizeHorCursor);
                break;

        case EllipseItem::RotationHandle :
                theView->setCursor(Qt::PointingHandCursor);  // TODO: Create rotation cursor
                break;

        case EllipseItem::RightHandle :
                theView->setCursor(Qt::SizeHorCursor);
                break;

        case EllipseItem::BottomLeftHandle :
                theView->setCursor(Qt::SizeBDiagCursor);
                break;

        case EllipseItem::BottomHandle :
                theView->setCursor(Qt::SizeVerCursor);
                break;

        case EllipseItem::BottomRightHandle :
                theView->setCursor(Qt::SizeFDiagCursor);
                break;

        case EllipseItem::MoveHandle :
                theView->setCursor(Qt::OpenHandCursor);
                break;

        default :
                theView->setCursor(Qt::ArrowCursor);
                break;
    }
}

QPointF EllipseItem::getNearestEdge(GraphicsSheet* view, const QPointF& scenePos) {
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


#if 0
void EllipseItem::paintCoordinates(GraphicsSheet* view, QPainter* painter) {
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

void EllipseItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * style, QWidget *widget) {
    // reset the selected state when painting - this is a workaround
    // to avoid that QGraphicsEllipseItem::paint draws the selection rectangle
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


void EllipseItem::writeExternal(QXmlStreamWriter& writer) {
    writer.writeStartElement("EllipseItem");
    writer.writeAttribute("xpos", QString::number(x()));
    writer.writeAttribute("ypos", QString::number(y()));
    writer.writeAttribute("xradius", QString::number(rect().width()/2));
    writer.writeAttribute("yradius", QString::number(rect().height()/2));
    writer.writeAttribute("rotation", QString::number(rotation()));
    writer.writeEndElement();
}


void EllipseItem::readExternal(QXmlStreamReader& reader) {
    qreal xpos = reader.attributes().value("xpos").toFloat();
    qreal ypos = reader.attributes().value("ypos").toFloat();
    qreal xradius = reader.attributes().value("xradius").toFloat();
    qreal yradius = reader.attributes().value("yradius").toFloat();
    qreal rotationAttr = reader.attributes().value("rotation").toFloat();

    setPos(xpos, ypos);
    setRect(-xradius, -yradius, xradius, yradius);
    setRotation(rotationAttr);
}
