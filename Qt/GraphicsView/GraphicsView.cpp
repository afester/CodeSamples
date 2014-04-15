/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */


#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QComboBox>
#include <QCheckBox>
#include <QDebug>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QScreen>
#include <QApplication>
#include <QtCore/qmath.h>
#include <QScrollbar>

#include "GraphicsView.h"
#include "LabelledComboBox.h"
#include "ScrollAreaLayout.h"


#define RULERHEIGHT 23
#define RULERWIDTH 23

GraphicsSheet::GraphicsSheet(QWidget* parent) : QGraphicsView(parent),
        drawScale(1.0), zoomScale(1.0), sceneSize(100, 100), landscape(true) {

	setRenderHint(QPainter::Antialiasing);
	setAutoFillBackground(true);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setViewportMargins(RULERWIDTH, RULERHEIGHT, 0, 0);

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

    setScene(new GraphicsScene());
}



void GraphicsSheet::drawBackground(QPainter * painter, const QRectF & rect) {
//	painter->fillRect(rect, QBrush(Qt::white));
}


void GraphicsSheet::setColor(const QColor& color) {
    viewColor = color;
}


QColor GraphicsSheet::getColor() {
    return viewColor;
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





GraphicsItem::GraphicsItem ( qreal x, qreal y, qreal width, qreal height, QGraphicsItem * parent) :
        QGraphicsRectItem(x, y, width, height, parent) {
}


void GraphicsItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) {
    GraphicsSheet* requestingView = dynamic_cast<GraphicsSheet*>(widget->parent());
    if (requestingView) {
        painter->setPen(QPen(requestingView->getColor(), 0));
    }

    painter->drawRect(rect());
}


class ColoredWidget : public QWidget {
public:
    ColoredWidget(const  QColor& color, QWidget* parent) : QWidget(parent) {
        QPalette pal;
        QBrush brush(color);
        brush.setStyle(Qt::SolidPattern);
        pal.setBrush(QPalette::Active, QPalette::Window, brush);
        setPalette(pal);
        setAutoFillBackground(true);
    }
};


ScaleEdgeWidget::ScaleEdgeWidget(QWidget* parent) : QWidget(parent) {
    setAutoFillBackground(true);
    setFont(QFont("Sans", 6));  // default font

    QPalette pal = palette();
    pal.setBrush(QPalette::Base, Qt::white);
    pal.setBrush(QPalette::Window, Qt::white);
    setPalette(pal);
}

void ScaleEdgeWidget::setUnit(const QString& theUnit) {
    unit = theUnit;
}

void ScaleEdgeWidget::paintEvent ( QPaintEvent * event ) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    p.setPen(Qt::darkGray);
    p.drawLine(0, 0, width(), 0);
    p.drawLine(1, 1, width(), 1);
    p.drawLine(0, 0, 0, height());
    p.drawLine(1, 1, 1, height());
    p.drawLine(1, 1, width(), height());

    QTextOption option;
    option.setAlignment(Qt::AlignRight);
    QRectF textBox(2, 1,
                   20, fontMetrics().height());
    //p.drawRect(textBox);
    p.drawText(textBox, unit, option);

    QRectF textBox2(2, 3 + fontMetrics().height(),
                    20, fontMetrics().height());
    //p.drawRect(textBox2);
    p.drawText(textBox2, unit);
}


ScaleWidget::ScaleWidget(QWidget* parent, GraphicsSheet* view, Direction dir) :
            QWidget(parent), theView(view), direction(dir),
            smallTicksSize(4), mediumTicksSize(6), largeTicksSize(8), offset(0) {
    setAutoFillBackground(true);
    setFont(QFont("Sans", 6));  // default font

    QPalette pal = palette();
    pal.setBrush(QPalette::Base, Qt::white); /// QColor(0xf8, 0xf8, 0xf8)); //Qt::white);
    pal.setBrush(QPalette::Window, Qt::white); // QColor(0xf8, 0xf8, 0xf8)); // Qt::white);
    pal.setColor(QPalette::Foreground, QColor(0x80, 0x80, 0x80)); // Qt::gray);
    setPalette(pal);
}


void ScaleWidget::setScale(float scale) {
    theScale = scale;
}


void ScaleWidget::setOffset(int value) {
    offset = value;
    repaint();
}


void ScaleWidget::paintEvent ( QPaintEvent * event ) {
    QPainter p(this);

    QFontMetrics fm = fontMetrics();


    if (direction == Vertical) {
        p.setPen(Qt::lightGray);
        p.drawLine(width() - 1, 0, width() - 1, height());

        p.setPen(Qt::darkGray);
        p.setRenderHint(QPainter::Antialiasing);
        p.drawLine(0, 0, 0, height());
        p.drawLine(1, 0, 1, height() - 2);

        qreal scale = theView->transform().m22() / theScale;
        QTextOption option;
        option.setAlignment(Qt::AlignRight);

        for (int y = 0;  y < (theView->viewport()->height() + offset ) / scale;  y++) {
            float ypos = y*scale - offset;

            if ( (y % 10) == 0) {
                p.drawLine(QPointF(20 - largeTicksSize, ypos),
                           QPointF(width() - 4, ypos));

                if (y > 0) {
                    QRectF textBox(0, ypos - fm.height(), 14, fm.height());
                    //p.drawRect(textBox);
                    p.drawText(textBox, QString::number(y / theScale), option);
                }
            } else if ( (y % 5) == 0) {
                p.drawLine(QPointF(20 - mediumTicksSize, ypos),
                           QPointF(width() - 4, ypos));
            } else {
                p.drawLine(QPointF(20 - smallTicksSize, ypos),
                           QPointF(width() - 4, ypos));
            }
        }
    } else {
        p.setPen(Qt::lightGray);
        p.drawLine(0, height() - 1, width(), height() - 1);

        p.setPen(Qt::darkGray);
        p.setRenderHint(QPainter::Antialiasing);
        p.drawLine(0, 0, width(), 0);
        p.drawLine(0, 1, width() - 2, 1);

        qreal scale = theView->transform().m11() / theScale;
        QTextOption option;
        option.setAlignment(Qt::AlignRight);

        for (int x = 0;  x < (theView->viewport()->width() + offset ) / scale;  x++) {
            float xpos = x*scale - offset;

            if ( (x % 10) == 0) {
                p.drawLine(QPointF(xpos, 19 - largeTicksSize),
                           QPointF(xpos, height() - 4));

                if (x > 0) {
                    QRectF textBox(xpos - 20, 3,
                            //(x-9)*scale, 0, // 22 - largeTicksSize - fm.height(),
                                    18, fm.height());
                    //p.drawRect(textBox);
                    p.drawText(textBox, QString::number(x / theScale), option);
                }
            } else if ( (x % 5) == 0) {
                p.drawLine(QPointF(xpos, 19 - mediumTicksSize),
                           QPointF(xpos, height() - 4));
            } else {
                p.drawLine(QPointF(xpos, 19 - smallTicksSize),
                           QPointF(xpos, height() - 4));
            }
        }
    }
}

#if 0

class CenterLayout : public QGridLayout {
public:
       CenterLayout(QWidget* parent, QWidget* center) : QGridLayout(parent) {
           setMargin(0);
           setHorizontalSpacing(0);
           setVerticalSpacing(0);

           setRowStretch(0, 1);
           setRowStretch(2, 1);
           setColumnStretch(0, 1);
           setColumnStretch(2, 1);

           addWidget(center, 1, 1);
       }

       void setGeometry(const QRect& r) {
           QGridLayout::setGeometry(r);
           //qDebug() << r;
       }
};


class CenterLayout : public QLayout {
    QWidget* centralWidget;
public:
       CenterLayout(QWidget* parent, QWidget* center) : QLayout(parent), centralWidget(center) {
//           setMargin(0);
/*           setHorizontalSpacing(0);
           setVerticalSpacing(0);

           setRowStretch(0, 1);
           setRowStretch(2, 1);
           setColumnStretch(0, 1);
           setColumnStretch(2, 1);

           addWidget(center, 1, 1);*/
       }

       virtual QSize sizeHint() const = 0;

       virtual void addItem(QLayoutItem *) = 0;

       virtual QLayoutItem *itemAt(int index) const = 0;

       virtual QLayoutItem *takeAt(int index) = 0;

       virtual int count() const {
           return 1;
       }

       void setGeometry(const QRect& r) {
           qDebug() << "LAYOUT:" << r;
           qDebug() << "  WDG:" << centralWidget->sizeHint();
           //QGridLayout::setGeometry(r);
           //qDebug() << r;
       }
};
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent) {

    setObjectName(QStringLiteral("MainWindow"));
    resize(1024, 768);

    graphicsSheet = new GraphicsSheet(this);
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(graphicsSheet);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0xa0, 0xa0, 0xa0));
    graphicsSheet->setGraphicsEffect(shadow);

    QLayout* layout = new ScrollAreaLayout();
    layout->addWidget(graphicsSheet);

    QWidget *centralwidget = new QWidget(this);
    centralwidget->setLayout(layout);
    setCentralWidget(centralwidget);

/*****************************************************************************/
    menubar = new QMenuBar(this);
    menubar->setObjectName(QStringLiteral("menubar"));
    menubar->setGeometry(QRect(0, 0, 401, 21));
    setMenuBar(menubar);

    statusbar = new QStatusBar(this);
    statusbar->setObjectName(QStringLiteral("statusbar"));
    setStatusBar(statusbar);

    toolBar = new QToolBar(this);
    toolBar->setObjectName(QStringLiteral("toolBar"));
    addToolBar(Qt::TopToolBarArea, toolBar);
/*****************************************************************************/

/* Initialize zoom, scale, and paper formats */
    graphicsSheet->addScale("1:10",  0.1);
    graphicsSheet->addScale("1:5",   0.2);
    graphicsSheet->addScale("1:2",   0.5);
    graphicsSheet->addScale("1:1",   1.0);
    graphicsSheet->addScale("2:1",   2.0);
    graphicsSheet->addScale("5:1",   5.0);
    graphicsSheet->addScale("10:1", 10.0);

    LabelledComboBox* scaleWidget = new LabelledComboBox(toolBar, "Scale: ");
    scaleWidget->getComboBox()->addItems(graphicsSheet->getScaleNames());
    toolBar->addWidget(scaleWidget);
    QObject::connect(scaleWidget->getComboBox(), SIGNAL(currentIndexChanged(int)),
                     graphicsSheet, SLOT(setScale(int)));

    graphicsSheet->addZoom("50%",  0.5);
    graphicsSheet->addZoom("75%",  0.75);
    graphicsSheet->addZoom("100%", 1.0);
    graphicsSheet->addZoom("125%", 1.25);
    graphicsSheet->addZoom("150%", 1.5);
    graphicsSheet->addZoom("200%", 2.0);

    LabelledComboBox* zoomWidget = new LabelledComboBox(toolBar, "Zoom: ");
    zoomWidget->getComboBox()->addItems(graphicsSheet->getZoomNames());
    toolBar->addWidget(zoomWidget);
    QObject::connect(zoomWidget->getComboBox(), SIGNAL(currentIndexChanged(int)),
                     graphicsSheet, SLOT(setZoom(int)));

    graphicsSheet->setUnit("mm");
    graphicsSheet->addSize("DIN A3", QSizeF(297.0, 420.0));
    graphicsSheet->addSize("DIN A4", QSizeF(210.0, 297.0));
    graphicsSheet->addSize("Letter", QSizeF(215.9, 279.4));
    graphicsSheet->addSize("DIN A5", QSizeF(148.0, 210.0));

    LabelledComboBox* sizeWidget = new LabelledComboBox(toolBar, "Sheet Size: ");
    sizeWidget->getComboBox()->addItems(graphicsSheet->getSizeNames());
    toolBar->addWidget(sizeWidget);
    QObject::connect(sizeWidget->getComboBox(), SIGNAL(currentIndexChanged(int)),
                     graphicsSheet, SLOT(setSize(int)));

    QCheckBox* checkbox = new QCheckBox("Landscape: ", toolBar);
    toolBar->addWidget(checkbox);
    QObject::connect(checkbox, SIGNAL(stateChanged(int)),
                     graphicsSheet, SLOT(setDirection(int)));

    QAction* actionInfo = new QAction("Info", this);
    toolBar->addAction(actionInfo);
    QObject::connect(actionInfo, SIGNAL(triggered(bool)),
    				 this, SLOT(printInfo()));

    zoomWidget->getComboBox()->setCurrentIndex(2);
    sizeWidget->getComboBox()->setCurrentIndex(3);
    scaleWidget->getComboBox()->setCurrentIndex(2);
    checkbox->setChecked(true);

    graphicsSheet->setColor(Qt::blue);  // A blue view
/*****************************************************************************/

    GraphicsItem* item = new GraphicsItem(10, 10, 50, 50);
    graphicsSheet->scene()->addItem(item);

    item = new GraphicsItem(0, 0, 5, 5);
    graphicsSheet->scene()->addItem(item);

    item = new GraphicsItem(225, 295, 5, 5);
    graphicsSheet->scene()->addItem(item);

    item = new GraphicsItem(125, 100, 200, 100);
    graphicsSheet->scene()->addItem(item);
}


void MainWindow::printInfo() {
	qDebug() << "\nLAYOUT INFO:\n====================";
	qDebug() << "   QGraphicsSheet: " << graphicsSheet->rect();
	qDebug() << "   viewport: " << graphicsSheet->viewport()->rect();
}


MainWindow::~MainWindow() {
}
