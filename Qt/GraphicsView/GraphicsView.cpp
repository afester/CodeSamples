/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

/* StackOverflow reference:
 * http://stackoverflow.com/questions/...
 */

#include <iostream>
#include <typeinfo>

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

QLabel* debugConsole;
int c = 0;

GraphicsView::GraphicsView(QWidget* parent) : QGraphicsView(parent) {

	setRenderHint(QPainter::Antialiasing);
	setAutoFillBackground(true);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);

	QScreen *srn = QApplication::screens().at(0);
	xDpi = (qreal)srn->logicalDotsPerInchX();
	// xDpi = 93;
	yDpi = (qreal)srn->logicalDotsPerInchY();

    setFrameStyle(0); // do not show a frame around the scroll area
}


void GraphicsView::drawBackground(QPainter * painter, const QRectF & rect) {
//	painter->fillRect(rect, QBrush(Qt::white));
}


void GraphicsView::setColor(const QColor& color) {
    viewColor = color;
}


QColor GraphicsView::getColor() {
    return viewColor;
}


void GraphicsView::updateSize() {
	QSizeF realSize = sceneSize;
	if (landscape) {
		realSize = QSize(sceneSize.height(), sceneSize.width());
	}

	scene()->setSceneRect(QRectF(0, 0, realSize.width(), realSize.height()));

	float xScaleDPI = xDpi / 25.4;
	float yScaleDPI = yDpi / 25.4;

	float effectiveScaleX = zoomScale * xScaleDPI;
	float effectiveScaleY = zoomScale * yScaleDPI;

	QTransform transform;
	transform.scale(effectiveScaleX, effectiveScaleY);
	setTransform(transform);

	updateGeometry();

	qDebug() << "DPI:" << xDpi << ", " << yDpi;
	qDebug() << "px/mm: " << xScaleDPI << ", " << yScaleDPI;
	qDebug() << "ZOOM SCALE:" << zoomScale;
	qDebug() << "EFFECTIVE SCALE:" << effectiveScaleX << ", " << effectiveScaleY;
	qDebug() << "SCENE RECT:" << sceneRect() << "/" << scene()->sceneRect();
	QRectF scaledScene(sceneRect().x(), sceneRect().y(),
					   sceneRect().width() * effectiveScaleX,
					   sceneRect().height() * effectiveScaleY);
	qDebug() << "SCALED SCENE RECT:" << scaledScene;
	qDebug() << "SCROLLAREA MIN SIZE:" << minimumSize();
	qDebug() << "SCROLLAREA MIN SIZE HINT:" << minimumSizeHint();
	qDebug() << "SCROLLAREA MAX SIZE: " << maximumSize();
	qDebug() << "SCROLLAREA SIZE: " << rect();
}


void GraphicsView::setZoom(float zoom) {
    zoomScale = zoom;
    updateSize();
}


void GraphicsView::setSize(const QSizeF& dimension) {
    sceneSize = dimension;
    updateSize();
}


void GraphicsView::setDirection(int idx) {
    if (idx == Qt::Checked) {
        landscape = true;
    } else {
        landscape = false;
    }
    updateSize();
}


QSize GraphicsView::sizeHint() const {
   // Default size calculation from QGraphicsSheet::sizeHint()
   QSizeF baseSize = matrix().mapRect(sceneRect()).size();
   baseSize += QSizeF(frameWidth() * 2, frameWidth() * 2);

   //   baseSize = baseSize.boundedTo((3 * QApplication::desktop()->size()) / 4);

   // before rounding through toSize(), add 0.5 to make sure to round upwards
   // See https://bugreports.qt-project.org/browse/QTBUG-37702
   QSize result = QSize(qCeil(baseSize.width()), qCeil(baseSize.height()));

   //result += QSize(4, 4);   // viewport margins are not yet considered!!!

   // qDebug() << "   RESULT:" << result;

   return result;
}


GraphicsView* GraphicsSheet::getView() {
    return view;
}


GraphicsScene* GraphicsSheet::getScene() {
    return scene;
}


GraphicsItem::GraphicsItem ( qreal x, qreal y, qreal width, qreal height, QGraphicsItem * parent) :
        QGraphicsRectItem(x, y, width, height, parent) {
}


void GraphicsItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) {
    GraphicsView* requestingView = dynamic_cast<GraphicsView*>(widget->parent());
    if (requestingView) {
        painter->setPen(requestingView->getColor());
    }

    painter->drawRect(rect());
}


class CenterLayout : public QGridLayout {
public:
	CenterLayout(QWidget* parent) : QGridLayout(parent) { }

	void setGeometry(const QRect& r) {
		qDebug() << "\n   LAYOUT rect:" << r;
		qDebug() << "   Item count :" << count();

		QGridLayout::setGeometry(r);

		QWidget* wdg = itemAtPosition(1, 1)->widget();
		qDebug() << "   Widget:" << wdg;
		qDebug() << "      rect(): " << wdg->rect();
		qDebug() << "	   minimumSize(): " << wdg->minimumSize();
		qDebug() << "	   minimumSizeHint(): " << wdg->minimumSizeHint();
		qDebug() << "	   maximumSize(): " << wdg->maximumSize();
		qDebug() << "	   sizeHint(): " << wdg->sizeHint();

		QGraphicsView* view = dynamic_cast<QGraphicsView*>(wdg);
		if (view) {
			qDebug() << "      sceneRect():" << view->sceneRect() << "/" << view->scene()->sceneRect();
			//QRectF scaledScene(sceneRect().x(), sceneRect().y(),
			//				   sceneRect().width() * effectiveScaleX,
			//				   sceneRect().height() * effectiveScaleY);
			//qDebug() << "SCALED SCENE RECT:" << scaledScene;
		}
		//wdg->setGeometry(wdg->x(), wdg->y(), wdg->height(), wdg->width() + 1);
	}
};


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


class ScaleEdgeWidget : public QWidget {
    QString unit;

public:
    ScaleEdgeWidget(QWidget* parent) : QWidget(parent) {
        setAutoFillBackground(true);
        setFont(QFont("Sans", 6));  // default font

        QPalette pal = palette();
        pal.setBrush(QPalette::Base, Qt::white);
        pal.setBrush(QPalette::Window, Qt::white);
        setPalette(pal);
    }

    void setUnit(const QString& theUnit) {
        unit = theUnit;
    }

protected:
    void paintEvent ( QPaintEvent * event ) {
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

};


ScaleWidget::ScaleWidget(QWidget* parent, GraphicsView* view, Direction dir) :
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

        qreal scale = theView->transform().m22();
        QTextOption option;
        option.setAlignment(Qt::AlignRight);

        for (int y = 0;  y < height() / scale;  y++) {
            if ( (y % 10) == 0) {
                p.drawLine(QPointF(20 - largeTicksSize, y*scale), QPointF(width() - 4, y*scale));

                if (y > 0) {
                    QRectF textBox(0, y * scale - fm.height(), 14, fm.height());
                    //p.drawRect(textBox);
                    p.drawText(textBox, QString::number(y / theScale), option);
                }
            } else if ( (y % 5) == 0) {
                p.drawLine(QPointF(20 - mediumTicksSize, y*scale), QPointF(width() - 4, y*scale));
            } else {
                p.drawLine(QPointF(20 - smallTicksSize, y*scale), QPointF(width() - 4, y*scale));
            }
        }
    } else {
        p.setPen(Qt::lightGray);
        p.drawLine(0, height() - 1, width(), height() - 1);

        p.setPen(Qt::darkGray);
        p.setRenderHint(QPainter::Antialiasing);
        p.drawLine(0, 0, width(), 0);
        p.drawLine(0, 1, width() - 2, 1);

        qreal scale = theView->transform().m11();
        QTextOption option;
        option.setAlignment(Qt::AlignRight);

        for (int x = 0;  x < width();  x++) {
            if ( (x % 10) == 0) {
                p.drawLine(QPointF(x*scale, 19 - largeTicksSize), QPointF(x*scale, height() - 4));

                if (x > 0) {
                    QRectF textBox(x*scale-20, 3,
                            //(x-9)*scale, 0, // 22 - largeTicksSize - fm.height(),
                                    18, fm.height());
                    //p.drawRect(textBox);
                    p.drawText(textBox, QString::number(x / theScale), option);
                }
            } else if ( (x % 5) == 0) {
                p.drawLine(QPointF(x*scale, 19 - mediumTicksSize), QPointF(x*scale, height() - 4));
            } else {
                p.drawLine(QPointF(x*scale, 19 - smallTicksSize), QPointF(x*scale, height() - 4));
            }
        }
    }
}


GraphicsSheet::GraphicsSheet(QWidget* parent) : QFrame(parent) {
    view = new GraphicsView(this);

    // create widget with fixed height of 20 px and maximum width of 200
    xScale = new ScaleWidget(this, view, ScaleWidget::Horizontal);
    xScale->setFixedHeight(23);

    // create widget with fixed width of 20 px and maximum height of 200
    yScale = new ScaleWidget(this, view, ScaleWidget::Vertical);
    yScale->setFixedWidth(23);

    edge = new ScaleEdgeWidget(this);
    edge->setFixedSize(23, 23);

    QGridLayout* layout = new QGridLayout();
    layout->setMargin(0);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);

    setLayout(layout);
    layout->addWidget(edge,  0, 0);
    layout->addWidget(xScale,  0, 1);

    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addWidget(yScale, 1);
    vbox->addStretch(0);

    layout->addLayout(vbox, 1, 0);

    layout->addWidget(view, 1, 1);

    QObject::connect(view->verticalScrollBar(), SIGNAL(valueChanged(int)),
                     this, SLOT(areaMoved()));
    QObject::connect(view->horizontalScrollBar(), SIGNAL(valueChanged(int)),
                     this, SLOT(areaMoved()));

    scene = new GraphicsScene();
    view->setScene(scene);
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
        view->setZoom(zoomLevels.at(idx));
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
        xScale->setScale(scaleLevels.at(idx));
        xScale->repaint();
        yScale->setScale(scaleLevels.at(idx));
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
        view->setSize(sizeDimensions[idx]);
    }
}


void GraphicsSheet::setDirection(int idx) {
    view->setDirection(idx);
}

void GraphicsSheet::areaMoved() {
    QPoint topLeft = view->viewport()->rect().topLeft();

    xScale->setOffset(view->horizontalScrollBar()->value() - topLeft.x());
    yScale->setOffset(view->verticalScrollBar()->value() - topLeft.y());
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent) {

    setObjectName(QStringLiteral("MainWindow"));
    resize(1024, 768);

    centralwidget = new QWidget(this);

    graphicsSheet = new GraphicsSheet(centralwidget);
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(graphicsSheet);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0xa0, 0xa0, 0xa0));
    graphicsSheet->setGraphicsEffect(shadow);

    QGridLayout* ml = new CenterLayout(centralwidget);
    ml->setRowStretch(0, 1);
    ml->setMargin(0);
    ml->setHorizontalSpacing(0);
    ml->setVerticalSpacing(0);

    ml->setRowStretch(2, 1);
    ml->setColumnStretch(0, 1);
    ml->setColumnStretch(2, 1);
    ml->addWidget(graphicsSheet, 1, 1);

    centralwidget->setLayout(ml);
    setCentralWidget(centralwidget);

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

    GraphicsItem* item = new GraphicsItem(10, 10, 50, 50);
    graphicsSheet->scene->addItem(item);
    item = new GraphicsItem(0, 0, 5, 5);
    graphicsSheet->scene->addItem(item);
    item = new GraphicsItem(225, 295, 5, 5);
    graphicsSheet->scene->addItem(item);
    item = new GraphicsItem(125, 100, 200, 100);
    graphicsSheet->scene->addItem(item);

    zoomWidget->getComboBox()->setCurrentIndex(2);
    sizeWidget->getComboBox()->setCurrentIndex(3);
    scaleWidget->getComboBox()->setCurrentIndex(2);
    checkbox->setChecked(true);

    graphicsSheet->getView()->setColor(Qt::blue);	// A blue view
}


void MainWindow::printInfo() {
	qDebug() << "\nLAYOUT INFO:\n====================";
	qDebug() << "   QGraphicsView: " << graphicsSheet->getView()->rect();
	qDebug() << "   viewport: " << graphicsSheet->getView()->viewport()->rect();
}


MainWindow::~MainWindow() {
}
