/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */


#include <QDebug>
#include <QLabel>
#include <QScreen>
#include <QtCore/qmath.h>
#include <QScrollBar>
#include <QMouseEvent>
#include <QHash>

#include "GraphicsSheet.h"
#include "ScaleWidget.h"
#include "ScaleEdgeWidget.h"
#include "Interactor.h"
#include "RectItem.h"
#include "Log.h"

#define RULERHEIGHT 23
#define RULERWIDTH 23

GraphicsSheet::GraphicsSheet(QWidget* parent) : QGraphicsView(parent),
        drawScale(1.0), zoomScale(1.0), sceneSize(100, 100), landscape(true),
        currentInteractor(0) {

	setRenderHint(QPainter::Antialiasing);
	setAutoFillBackground(true);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setViewportMargins(RULERWIDTH, RULERHEIGHT, 0, 0);
    setMouseTracking(true);       // Enable mouse tracking to report mouse position

	QScreen *srn = QApplication::screens().at(0);
	xDpi = (qreal)srn->logicalDotsPerInchX();
	// xDpi = 93;
	yDpi = (qreal)srn->logicalDotsPerInchY();

    setFrameStyle(0); // do not show a frame around the scroll area

    // create widget with fixed height of 20 px and maximum width of 200
    xScale = new ScaleWidget(this, this, ScaleWidget::Horizontal);
    xScale->setFixedHeight(RULERHEIGHT);

    // create widget with fixed width of 20 px and maximum height of 200
    yScale = new ScaleWidget(this, this, ScaleWidget::Vertical);
    yScale->setFixedWidth(RULERWIDTH);

    edge = new ScaleEdgeWidget(this);
    edge->setFixedSize(RULERHEIGHT, RULERWIDTH);

    QObject::connect(verticalScrollBar(), SIGNAL(valueChanged(int)),
                     this, SLOT(areaMoved()));
    QObject::connect(horizontalScrollBar(), SIGNAL(valueChanged(int)),
                     this, SLOT(areaMoved()));

    setScene(new QGraphicsScene());

    // TODO: There are some strange artefacts when an item is moved while
    // partially hidden by another item. It should be sufficient to
    // define a reasonable boundingRect, but it seems that this is not sufficient ...
    setViewportUpdateMode(FullViewportUpdate);
}


void GraphicsSheet::drawBackground(QPainter * painter, const QRectF & rect) {
//	painter->fillRect(rect, QBrush(Qt::white));
}


void GraphicsSheet::addPoint(const QString& name, const QPointF& p) {
    points.insert(name, p);
}


void GraphicsSheet::drawCoordinateSystem(QPainter* painter, const QTransform& t, const QColor& c) {

    QPointF O = t.map(QPointF(0,0));

    QPointF X = t.map(QPointF(50, 0));
    QPointF Xa1 = t.map(QPointF(45, -3));
    QPointF Xa2 = t.map(QPointF(45, 3));

    QPointF Y = t.map(QPointF(0, 50));
    QPointF Ya1 = t.map(QPointF(-3, 45));
    QPointF Ya2 = t.map(QPointF(3, 45));

    painter->setPen(c);

    painter->drawLine(O, X);
    painter->drawLine(X, Xa1);
    painter->drawLine(X, Xa2);

    painter->drawLine(O, Y);
    painter->drawLine(Y, Ya1);
    painter->drawLine(Y, Ya2);
}

void GraphicsSheet::drawPoints(QPainter* painter, const QHash<QString, QPointF>& points) {
    painter->setFont(QFont("Sans", 6));
    QHash<QString, QPointF>::const_iterator i = points.constBegin();
    while (i != points.constEnd()) {
        painter->setPen(Qt::red);
        painter->drawEllipse(i.value(), 1, 1);

        QString out;
        QTextStream s(&out);
        s << i.key() << "(" << i.value().x() << "/" << i.value().y() << ")";

        painter->setPen(Qt::black);
        painter->drawText(i.value().x(), i.value().y() - 5, out);

        ++i;
    }
}

void GraphicsSheet::drawForeground(QPainter * painter, const QRectF & rect) {
    // QGraphicsView::paintEvent() only repaints the boundingRect() area of the item.
    // we therefore need to properly add a margin inside the item's boundingRect() method.

    painter->save();
    QGraphicsItem* item;
    foreach(item, scene()->selectedItems()) {
        InteractableItem* edItem = dynamic_cast<InteractableItem*>(item);
        if (edItem) {

            // set the painter's transformation so that the paint operations are
            // executed in item coordinate space
            QTransform trans = item->deviceTransform(QTransform());
            painter->setWorldTransform(trans, true);

            // Paint the selection border
            edItem->paintSelectedBorder(this, painter);

            // paint the edit handles
            edItem->paintHandles(this, painter, RectItem::AllHandlesMask);

#if 0
            // DEBUG: draw the shape of the item
            painter->setPen(QPen(Qt::blue, 0, Qt::DotLine));
            painter->setBrush(Qt::NoBrush);
            painter->drawPath(item->shape());
#endif
        }
    }
    painter->restore();

    drawPoints(painter, points);

    drawCoordinateSystem(painter);

    QTransform t;

    // Moves the coordinate system dx along the x axis and dy along the y axis
    t.translate(163, 57);

//    qDebug() << t;
    drawCoordinateSystem(painter, t, Qt::red);

    QHash<QString, QPointF> mapped;
    mapped.insert("P1", QPointF(10, 10));
    mapped.insert("P1'", t.map(QPointF(10, 10)));
    drawPoints(painter, mapped);

//    t.rotate(-30);
//    drawCoordinateSystem(painter, t, Qt::blue);
}


void GraphicsSheet::setScaleBackground(const QColor& color) {
    QPalette pal = xScale->palette();
    pal.setBrush(QPalette::Base, color);
    pal.setBrush(QPalette::Window, color);
    xScale->setPalette(pal);

    pal = yScale->palette();
    pal.setBrush(QPalette::Base, color);
    pal.setBrush(QPalette::Window, color);
    yScale->setPalette(pal);

    pal = edge->palette();
    pal.setBrush(QPalette::Base, color);
    pal.setBrush(QPalette::Window, color);
    edge->setPalette(pal);
}


void GraphicsSheet::updateSize() {
	QSizeF realSize = sceneSize;
	if (landscape) {
		realSize = QSize(sceneSize.height(), sceneSize.width());
	}

	scene()->setSceneRect(QRectF(0, 0, realSize.width() / drawScale, realSize.height() / drawScale));

	float xScaleDPI = xDpi / 25.4;
	float yScaleDPI = yDpi / 25.4;

	float effectiveScaleX = zoomScale * xScaleDPI * drawScale;
	float effectiveScaleY = zoomScale * yScaleDPI * drawScale;

	QTransform transform;
	transform.scale(effectiveScaleX, effectiveScaleY);
	setTransform(transform);

	updateGeometry();
}


void GraphicsSheet::setZoom(float zoom) {
    zoomScale = zoom;
    updateSize();
}


void GraphicsSheet::setScale(float scale) {
    drawScale = scale;
    updateSize();
}


void GraphicsSheet::setSize(const QSizeF& dimension) {
    sceneSize = dimension;
    updateSize();
}


void GraphicsSheet::setDirection(int idx) {
    if (idx == Qt::Checked) {
        landscape = true;
    } else {
        landscape = false;
    }
    updateSize();
}


QSize GraphicsSheet::sizeHint() const {
   // Default size calculation from QGraphicsSheet::sizeHint()
   QSizeF baseSize = matrix().mapRect(sceneRect()).size();
   baseSize += QSizeF(frameWidth() * 2, frameWidth() * 2);

   // before rounding through toSize(), add 0.5 to make sure to round upwards
   // See https://bugreports.qt-project.org/browse/QTBUG-37702
   QSize result = QSize(qCeil(baseSize.width()), qCeil(baseSize.height()));

   result += QSize(RULERWIDTH, RULERHEIGHT);   // viewport margins are not yet considered!!!

   //qDebug() << "   RESULT:" << result;

   return result;
}


void GraphicsSheet::resizeEvent ( QResizeEvent * event ) {
    QGraphicsView::resizeEvent(event);

    xScale->setGeometry(RULERWIDTH, 0, viewport()->width(), xScale->height());
    yScale->setGeometry(0, RULERHEIGHT, yScale->width(), viewport()->height());
}


void GraphicsSheet::setUnit(const QString& unit) {
    edge->setUnit(unit);
}

void GraphicsSheet::addZoom(const QString& name, float level) {
    zoomNames.append(name);
    zoomLevels.append(level);
}


QStringList GraphicsSheet::getZoomNames() const {
    return zoomNames;
}


void GraphicsSheet::setZoom(int idx) {
    if (idx >= 0 && idx < zoomLevels.size()) {
        setZoom(zoomLevels.at(idx));
    }
}


void GraphicsSheet::addScale(const QString& name, float level) {
    scaleNames.append(name);
    scaleLevels.append(level);
}


QStringList GraphicsSheet::getScaleNames() const {
    return scaleNames;
}


void GraphicsSheet::setScale(int idx) {
    if (idx >= 0 && idx < scaleLevels.size()) {
        setScale(scaleLevels.at(idx));

        xScale->setScale(drawScale);
        xScale->repaint();

        yScale->setScale(drawScale);
        yScale->repaint();

    }
}

void GraphicsSheet::addSize(const QString& name, const QSizeF& size) {
    sizeNames.append(name);
    sizeDimensions.append(size);
}


QStringList GraphicsSheet::getSizeNames() const {
    return sizeNames;
}


void GraphicsSheet::setSize(int idx) {
    if (idx >= 0 && idx < sizeDimensions.size()) {
        setSize(sizeDimensions[idx]);
    }
}


void GraphicsSheet::areaMoved() {
    QPoint topLeft = viewport()->rect().topLeft();

    xScale->setOffset(horizontalScrollBar()->value() - topLeft.x());
    yScale->setOffset(verticalScrollBar()->value() - topLeft.y());
}



void GraphicsSheet::setInteractor(Interactor* interactor) {
    this->currentInteractor = interactor;
    if (interactor != 0) {
        interactor->setView(this);
    }
}


Interactor* GraphicsSheet::getInteractor() {
    return currentInteractor;
}

InteractableItem* GraphicsSheet::getFocusItem() const {
    if (scene()->selectedItems().size() > 0) {
        return dynamic_cast<InteractableItem*>(scene()->selectedItems().at(0));  // TODO!
    }
    return 0;
}

void GraphicsSheet::mousePressEvent ( QMouseEvent * event ){
    if (currentInteractor) {
        currentInteractor->mousePressEvent(event);
        if (!event->isAccepted()) {
            QGraphicsView::mousePressEvent(event);
        }
    }
}


void GraphicsSheet::mouseMoveEvent ( QMouseEvent * event ) {
    float xSnapped = xScale->snapToTick(event->x() + horizontalScrollBar()->value());
    float ySnapped = yScale->snapToTick(event->y() + verticalScrollBar()->value());
    // qDebug() << "Scene coords: " << QPointF(xSnapped, ySnapped);

    xScale->setPos(xSnapped);
    yScale->setPos(ySnapped);

    if (currentInteractor) {
        currentInteractor->mouseMoveEvent(event);
        if (!event->isAccepted()) {
            QGraphicsView::mouseMoveEvent(event);
        }
    }
}


void GraphicsSheet::mouseReleaseEvent ( QMouseEvent * event ){
    if (currentInteractor) {
        currentInteractor->mouseReleaseEvent(event);
        if (!event->isAccepted()) {
            QGraphicsView::mouseReleaseEvent(event);
        }
    }
}


void GraphicsSheet::wheelEvent(QWheelEvent * event ) {
    if (currentInteractor) {
        currentInteractor->wheelEvent(event);
        if (!event->isAccepted()) {
            QGraphicsView::wheelEvent(event);
        }
    }
}


void GraphicsSheet::paste() {
//    Log::log(Log::DEBUG, "GraphicsSheet") << "    PASTE";

    if (currentInteractor) {
        currentInteractor->paste();
    }
}
