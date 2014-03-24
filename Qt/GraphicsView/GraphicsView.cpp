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

#include "GraphicsView.h"
#include "LabelledComboBox.h"

QLabel* debugConsole;
int c = 0;

GraphicsView::GraphicsView(QWidget* parent) : QGraphicsView(parent) {
	setRenderHint(QPainter::Antialiasing);
 //   setViewportMargins(30, 30, 0, 0);

	// setAlignment(Qt::AlignTop);

	QScreen *srn = QApplication::screens().at(0);
	//xDpi = (qreal)srn->physicalDotsPerInchX();
	//yDpi = (qreal)srn->physicalDotsPerInchY();
	xDpi = (qreal)srn->logicalDotsPerInchX();
	yDpi = (qreal)srn->logicalDotsPerInchY();
}


void GraphicsView::drawBackground(QPainter * painter, const QRectF & rect) {
/*	QString outStr;
	QTextStream out(&outStr);
	out << c++ << "  GraphicsView::sceneRect() " << sceneRect().width() << "/" << sceneRect().height()
	           << "\n  GraphicsScene::sceneRect() " << scene()->sceneRect().width() << "/" << scene()->sceneRect().height();
	debugConsole->setText(outStr);
*/
	//const int radius = 100;
	//QLinearGradient gradient(0, 0, 0, 150);
	//gradient.setCenter(radius, radius);
	//gradient.setFocalPoint(radius, radius);
	//gradient.setRadius(radius);
	//gradient.setColorAt(0, Qt::white);
	//gradient.setColorAt(1, Qt::darkGray);

	//painter->setCompositionMode(QPainter::CompositionMode_Source);
//	painter->setBrush(gradient);
	painter->fillRect(rect, QBrush(Qt::white));
	//painter->fillRect(5, 5, 145, 145, gradient);
	//painter->fillRect(0, 0, 145, 145, QBrush(Qt::white));
	//painter->drawEllipse(50, 50, 120, 120);

//	painter->fillRect(rect, QBrush(Qt::lightGray));
//	painter->fillRect(0, 0, 230, 300, QBrush(Qt::white));
}

void GraphicsView::paintEvent(QPaintEvent * event) {
	QGraphicsView::paintEvent(event);
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

//	QSize viewportSize((sceneRect().width() * effectiveScaleX)  + 13,	// +1 pixel
//			 	 	   (sceneRect().height() * effectiveScaleY) + 13);	// to overcome rounding issues


//	qDebug() << "VIEWPORT SIZE:" << viewportSize;

//	viewport()->setMaximumSize(viewportSize);

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


void GraphicsView::setZoom(int idx) {
	static float values[] = {0.25, 0.5, 1.0, 2.0};

	if (idx >= 0 && idx < sizeof(values)) {
		zoomScale = values[idx];
		updateSize();
	}
}


void GraphicsView::setSize(int idx) {
	static QSizeF sizes[] = {QSizeF(297.0, 420.0),  // mm (A3)
							 QSizeF(210.0, 297.0),  // mm (A4)
							 QSizeF(148, 210) };    // mm (A5)

	if (idx >= 0 && idx < sizeof(sizes)) {
		sceneSize = sizes[idx];
		updateSize();
	}
}


void GraphicsView::setDirection(int idx) {
	if (idx == Qt::Checked) {
		landscape = true;
	} else {
		landscape = false;
	}
	updateSize();
}



void GraphicsView::setupViewport ( QWidget * viewport ) {

}

QSize GraphicsView::sizeHint() const {
   // Default size calculation from QGraphicsView::sizeHint()
   QSizeF baseSize = matrix().mapRect(sceneRect()).size();
   baseSize += QSizeF(frameWidth() * 2, frameWidth() * 2);
   baseSize = baseSize.boundedTo((3 * QApplication::desktop()->size()) / 4);
   // qDebug() << "   BASE SIZE:" << baseSize;

   // before rounding through toSize(), add 0.5 to make sure to round upwards
   // See https://bugreports.qt-project.org/browse/QTBUG-37702
   baseSize += QSizeF(0.5, 0.5);

   QSize result = baseSize.toSize();
   // qDebug() << "   RESULT:" << result;

   return result;
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
#if 0
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), scene(new QGraphicsScene()) {


    //// green background palette for testing
    QPalette green;
    QBrush brush1(Qt::green);
    brush1.setStyle(Qt::SolidPattern);
    green.setBrush(QPalette::Active, QPalette::Base, brush1);
    green.setBrush(QPalette::Active, QPalette::Window, brush1);
    green.setBrush(QPalette::Inactive, QPalette::Base, brush1);
    green.setBrush(QPalette::Inactive, QPalette::Window, brush1);
    green.setBrush(QPalette::Disabled, QPalette::Base, brush1);
    green.setBrush(QPalette::Disabled, QPalette::Window, brush1);

    QPalette red;
    QBrush brush(Qt::lightGray);
    brush.setStyle(Qt::SolidPattern);
    red.setBrush(QPalette::Active, QPalette::Base, brush);
    red.setBrush(QPalette::Active, QPalette::Window, brush);
    red.setBrush(QPalette::Inactive, QPalette::Base, brush);
    red.setBrush(QPalette::Inactive, QPalette::Window, brush);
    red.setBrush(QPalette::Disabled, QPalette::Base, brush);
    red.setBrush(QPalette::Disabled, QPalette::Window, brush);

    QPalette yellow;
    QBrush brush3(Qt::yellow);
    brush3.setStyle(Qt::SolidPattern);
    yellow.setBrush(QPalette::Active, QPalette::Base, brush3);
    yellow.setBrush(QPalette::Active, QPalette::Window, brush3);
    yellow.setBrush(QPalette::Inactive, QPalette::Base, brush3);
    yellow.setBrush(QPalette::Inactive, QPalette::Window, brush3);
    yellow.setBrush(QPalette::Disabled, QPalette::Base, brush3);
    yellow.setBrush(QPalette::Disabled, QPalette::Window, brush3);


    setObjectName(QStringLiteral("MainWindow"));
    resize(640, 480);

    actionX = new QAction(this);
    actionX->setObjectName(QStringLiteral("actionX"));

    centralwidget = new QWidget();

    QFrame* graphicsWidget = new QFrame(centralwidget);
    graphicsWidget->setObjectName(QStringLiteral("frameWidget"));

    QGridLayout* ml = new CenterLayout(centralwidget);
    ml->setRowStretch(0, 1);
    ml->setMargin(0);
    ml->setHorizontalSpacing(0);
    ml->setVerticalSpacing(0);

    ml->setRowStretch(2, 1);
    ml->setColumnStretch(0, 1);
    ml->setColumnStretch(2, 1);
    ml->addWidget(graphicsWidget, 1, 1);
    centralwidget->setLayout(ml);

	QPalette palette;
	palette.setColor(QPalette::Foreground, QColor(0xa0, 0xa0, 0xa0));
	graphicsWidget->setPalette(palette);

    graphicsWidget->setFrameStyle(QFrame::Box);
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(graphicsWidget);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0xa0, 0xa0, 0xa0));
    graphicsWidget->setGraphicsEffect(shadow);
    graphicsWidget->setAutoFillBackground(true);

    QGridLayout* layout = new QGridLayout();
    graphicsWidget->setLayout(layout);

    layout->setMargin(0);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);

    QWidget* topHeader = new QWidget(graphicsWidget);
    topHeader->setFixedHeight(30);
    topHeader->setPalette(green);
    topHeader->setAutoFillBackground(true);
//    topHeader->setMaximumWidth(430);

    QWidget* leftHeader = new QWidget(graphicsWidget);
    leftHeader->setFixedWidth(30);
    leftHeader->setPalette(green);
    leftHeader->setAutoFillBackground(true);
//    leftHeader->setMaximumHeight(630);

    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addWidget(leftHeader, 1);
    vbox->addStretch(0);

    view = new GraphicsView(graphicsWidget);
    view->setObjectName(QStringLiteral("view"));

    layout->addWidget(topHeader, 0, 1);
    layout->addLayout(vbox, 1, 0);
    // layout->addWidget(leftHeader, 1, 0);
    layout->addWidget(view, 1, 1);

/*******************************/

    setCentralWidget(centralwidget);

    view->setFrameStyle(0);	// do not show a frame around the scroll area

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

    toolBar->addAction(actionX);

    QMetaObject::connectSlotsByName(this);

/**********************************************/

    debugConsole = new QLabel();
    debugConsole->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    debugConsole->setAlignment(Qt::AlignTop);
    debugConsole->setFixedHeight(200);
    debugConsole->setFixedWidth(200);
    debugConsole->show();

    LabelledComboBox* zoomWidget = new LabelledComboBox(toolBar, "Scale: ");
    zoomWidget->getComboBox()->addItem("25%");
    zoomWidget->getComboBox()->addItem("50%");
    zoomWidget->getComboBox()->addItem("100%");
    zoomWidget->getComboBox()->addItem("200%");
    toolBar->addWidget(zoomWidget);
    QObject::connect(zoomWidget->getComboBox(), SIGNAL(currentIndexChanged(int)),
    				 view, SLOT(setZoom(int)));

    LabelledComboBox* sizeWidget = new LabelledComboBox(toolBar, "Size: ");
    sizeWidget->getComboBox()->addItem("DIN A3");
    sizeWidget->getComboBox()->addItem("DIN A4");
    sizeWidget->getComboBox()->addItem("DIN A5");
    toolBar->addWidget(sizeWidget);
    QObject::connect(sizeWidget->getComboBox(), SIGNAL(currentIndexChanged(int)),
    				 view, SLOT(setSize(int)));

    QCheckBox* checkbox = new QCheckBox("Landscape: ", toolBar);
    toolBar->addWidget(checkbox);
    QObject::connect(checkbox, SIGNAL(stateChanged(int)),
    				 view, SLOT(setDirection(int)));

    QAction* actionInfo = new QAction("Info", this);
    toolBar->addAction(actionInfo);
    QObject::connect(actionInfo, SIGNAL(triggered(bool)),
    				 this, SLOT(printInfo()));



/*******************************************/
#if 0
    // set up a graphics scene
    QGraphicsRectItem* paper = new QGraphicsRectItem(0, 0, 230, 300);
    paper->setBrush(Qt::white);
    paper->setPen(Qt::NoPen); // QPen(Qt::darkGray));
    QGraphicsDropShadowEffect* eff = new QGraphicsDropShadowEffect();
    paper->setGraphicsEffect(eff);
    paper->setFlag(QGraphicsItem::ItemClipsChildrenToShape); // items are NOT children of this item by default!
    scene->addItem(paper);

    QGraphicsRectItem* item2 = new QGraphicsRectItem(0, -10, 230, 10);
    scene->addItem(item2);
    item2 = new QGraphicsRectItem(-10, 0, 10, 300);
    scene->addItem(item2);

    GraphicsItem* item = new GraphicsItem(10, 10, 50, 50);
    scene->addItem(item);
    item = new GraphicsItem(0, 0, 5, 5);
    scene->addItem(item);
    item = new GraphicsItem(225, 295, 5, 5);
    scene->addItem(item);
    item = new GraphicsItem(125, 100, 200, 100, paper);
    scene->addItem(item);
#endif

    view->setAlignment(Qt::AlignLeft | Qt::AlignTop);


//    scene->setSceneRect(0, 0, 400, 600);
//    view->viewport()->setMaximumSize(400, 600);
//   view->setMaximumSize(440, 640);

/*
    QWidget* topRuler = new QWidget(view);
    topRuler->setPalette(yellow);
    topRuler->setAutoFillBackground(true);
    topRuler->setFixedWidth(400);
    topRuler->setFixedHeight(30);
    topRuler->move(30, 0);

    QWidget* leftRuler = new QWidget(view);
    leftRuler->setPalette(yellow);
    leftRuler->setAutoFillBackground(true);
    leftRuler->setFixedWidth(30);
    leftRuler->setFixedHeight(600);
    leftRuler->move(0, 30);
*/
    GraphicsItem* item = new GraphicsItem(10, 10, 50, 50);
    scene->addItem(item);
    item = new GraphicsItem(0, 0, 5, 5);
    scene->addItem(item);
    item = new GraphicsItem(225, 295, 5, 5);
    scene->addItem(item);
    item = new GraphicsItem(125, 100, 200, 100);
    scene->addItem(item);


// sceneRect does not affect clipping! Items still paint outside the sceneRect if necessary.
//    scene->setSceneRect(-15, -15, 260, 320);
    view->setScene(scene);

    // initialize the sizes
    view->setSize(1);					// A4
    view->setDirection(Qt::Unchecked);	// portrait
    view->setZoom(2);					// 100%

#if 0
/*	QVBoxLayout* vbox = new QVBoxLayout();
	vbox->addWidget(leftHeader, 1);
	vbox->addStretch(0);
	//layout->addWidget(leftHeader, 1, 0);
	layout->addLayout(vbox, 1, 0);
*/


    QWidget* drawArea = ui->view->viewport();
    QGridLayout* layout = new QGridLayout();

    QWidget* mainWidget = new QWidget();

    QWidget* top = new QWidget(mainWidget);
    top->setAutoFillBackground(true);
    top->setFixedHeight(10);
    QPalette pal = top->palette();
    pal.setColor(QPalette::Window, Qt::yellow);
    top->setPalette(pal);

    QWidget* left = new QWidget(mainWidget);
    left->setAutoFillBackground(true);
    left->setFixedWidth(10);
    QPalette pal2 = left->palette();
    pal2.setColor(QPalette::Window, Qt::green);
    left->setPalette(pal2);

    mainWidget->setLayout(layout);
    //layout->addWidget(new QLabel("Horizontal label", mainWidget), 0, 0);
    //layout->addWidget(drawArea, 1, 0);
    layout->addWidget(top, 0, 1);
    layout->addWidget(left, 1, 0);
  //  layout->addWidget(drawArea, 1, 1);


    ui->view->setViewport(mainWidget);
#endif

    view->setColor(Qt::blue);	// A blue view
}
#endif

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



class ScaleWidget : public QWidget {

public:
    enum Direction{Vertical, Horizontal};

    ScaleWidget(QWidget* parent, GraphicsView* view, Direction dir) :
            QWidget(parent), theView(view), direction(dir) {
        setAutoFillBackground(true);
        setFont(QFont("Sans", 6));  // default font

        QPalette pal = palette();
        pal.setBrush(QPalette::Active, QPalette::Base, Qt::white);
        pal.setBrush(QPalette::Active, QPalette::Window, Qt::white);
        pal.setColor(QPalette::Active, QPalette::Foreground, QColor(0xA0, 0xA0, 0xA0)); // Qt::gray);
        setPalette(pal);
    }

protected:
    void paintEvent ( QPaintEvent * event ) {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        QFontMetrics fm = fontMetrics();

        if (direction == Vertical) {
            qreal scale = theView->transform().m22();
            QTextOption option;
            option.setAlignment(Qt::AlignRight);

            for (int y = 0;  y < height() / scale;  y++) {
                if ( (y % 10) == 0) {
                    p.drawLine(QPointF(12, y*scale), QPointF(width(), y*scale));

                    if (y > 0) {
                        QRectF textBox(0, y * scale - fm.height(), 14, fm.height());
                        //p.drawRect(textBox);
                        p.drawText(textBox, QString::number(y), option); // yy / (10*mm2pix)), option);
                    }
                } else {
                    p.drawLine(QPointF(16, y*scale), QPointF(width(), y*scale));

                }
            }
        } else {
            qreal scale = theView->transform().m11();
            QTextOption option;
            option.setAlignment(Qt::AlignRight);

            for (int x = 0;  x < width();  x++) {
                if ( (x % 10) == 0) {
                    p.drawLine(QPointF(x*scale, 6), QPointF(x*scale, height()));
int largeTicksSize = 5;

                    if (x > 0) {
                        QRectF textBox(x*scale-20, 0,
                                //(x-9)*scale, 0, // 22 - largeTicksSize - fm.height(),
                                        18, fm.height());
                        //p.drawRect(textBox);
                        p.drawText(textBox, QString::number(x), option);
                    }
                } else {
                    p.drawLine(QPointF(x*scale, 12), QPointF(x*scale, height()));
                }
            }
        }
    }

private:
    GraphicsView* theView;
    Direction direction;
};



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), scene(new QGraphicsScene()) {

    setObjectName(QStringLiteral("MainWindow"));
    resize(1024, 768);

    centralwidget = new QWidget(this);

    QFrame* graphicsWidget = new QFrame(centralwidget);
    graphicsWidget->setObjectName(QStringLiteral("frameWidget"));
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(graphicsWidget);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0xa0, 0xa0, 0xa0));
    graphicsWidget->setGraphicsEffect(shadow);

    QGridLayout* ml = new CenterLayout(centralwidget);
    ml->setRowStretch(0, 1);
    ml->setMargin(0);
    ml->setHorizontalSpacing(0);
    ml->setVerticalSpacing(0);

    ml->setRowStretch(2, 1);
    ml->setColumnStretch(0, 1);
    ml->setColumnStretch(2, 1);
    ml->addWidget(graphicsWidget, 1, 1);

    centralwidget->setLayout(ml);
    setCentralWidget(centralwidget);

/*********************/
    QGridLayout* layout = new QGridLayout();
    layout->setMargin(0);
    layout->setHorizontalSpacing(0);
    layout->setVerticalSpacing(0);

    view = new GraphicsView(graphicsWidget);
    view->setObjectName(QStringLiteral("view"));
    view->setFrameStyle(0); // do not show a frame around the scroll area

    // create widget with fixed height of 20 px and maximum width of 200
    QWidget* topHeader = new ScaleWidget(graphicsWidget, view, ScaleWidget::Horizontal);
//    topHeader->setMaximumWidth(200);
    topHeader->setFixedHeight(20);

    // create widget with fixed width of 20 px and maximum height of 200
    QWidget* leftHeader = new ScaleWidget(graphicsWidget, view, ScaleWidget::Vertical);
    leftHeader->setFixedWidth(23);
//    leftHeader->setMaximumHeight(200);


    graphicsWidget->setLayout(layout);
    layout->addWidget(topHeader,  0, 1);

    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addWidget(leftHeader, 1);
    vbox->addStretch(0);
    // layout->addWidget(leftHeader, 1, 0);
    layout->addLayout(vbox, 1, 0);

    layout->addWidget(view, 1, 1);

/************************************/

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


    LabelledComboBox* zoomWidget = new LabelledComboBox(toolBar, "Scale: ");
    zoomWidget->getComboBox()->addItem("25%");
    zoomWidget->getComboBox()->addItem("50%");
    zoomWidget->getComboBox()->addItem("100%");
    zoomWidget->getComboBox()->addItem("200%");
    toolBar->addWidget(zoomWidget);
    QObject::connect(zoomWidget->getComboBox(), SIGNAL(currentIndexChanged(int)),
    				 view, SLOT(setZoom(int)));

    LabelledComboBox* sizeWidget = new LabelledComboBox(toolBar, "Size: ");
    sizeWidget->getComboBox()->addItem("DIN A3");
    sizeWidget->getComboBox()->addItem("DIN A4");
    sizeWidget->getComboBox()->addItem("DIN A5");
    toolBar->addWidget(sizeWidget);
    QObject::connect(sizeWidget->getComboBox(), SIGNAL(currentIndexChanged(int)),
    				 view, SLOT(setSize(int)));

    QCheckBox* checkbox = new QCheckBox("Landscape: ", toolBar);
    toolBar->addWidget(checkbox);
    QObject::connect(checkbox, SIGNAL(stateChanged(int)),
    				 view, SLOT(setDirection(int)));

    QAction* actionInfo = new QAction("Info", this);
    toolBar->addAction(actionInfo);
    QObject::connect(actionInfo, SIGNAL(triggered(bool)),
    				 this, SLOT(printInfo()));

    view->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    GraphicsItem* item = new GraphicsItem(10, 10, 50, 50);
    scene->addItem(item);
    item = new GraphicsItem(0, 0, 5, 5);
    scene->addItem(item);
    item = new GraphicsItem(225, 295, 5, 5);
    scene->addItem(item);
    item = new GraphicsItem(125, 100, 200, 100);
    scene->addItem(item);

    view->setScene(scene);

    // initialize the sizes
    view->setSize(2);
    view->setDirection(Qt::Checked);
    view->setZoom(2);

    view->setColor(Qt::blue);	// A blue view
}


void MainWindow::printInfo() {
	qDebug() << "\nLAYOUT INFO:\n====================";
	qDebug() << "   QGraphicsView: " << view->rect();
	qDebug() << "   viewport: " << view->viewport()->rect();
}


MainWindow::~MainWindow() {
    delete scene;
}
