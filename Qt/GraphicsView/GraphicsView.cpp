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
#include <QDebug>
#include <QLabel>
#include <QGraphicsDropShadowEffect>

#include "GraphicsView.h"

QLabel* debugConsole;
int c = 0;

GraphicsView::GraphicsView(QWidget* parent) : QGraphicsView(parent) {
	setRenderHint(QPainter::Antialiasing);
 //   setViewportMargins(30, 30, 0, 0);

	// setAlignment(Qt::AlignTop);
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

void GraphicsView::setZoom(int idx) {
	static float values[] = {0.25, 0.5, 1.0, 2.0};

	if (idx >= 0 && idx < sizeof(values)) {
		qDebug() << values[idx];
		QTransform scale;
		scale.scale(values[idx], values[idx]);
		setTransform(scale);
	}

	switch(idx) {
		case 0 : viewport()->setMaximumSize(100, 150); break;
		case 1 : viewport()->setMaximumSize(200, 300); break;
		case 2 : viewport()->setMaximumSize(400, 600); break;
		case 3 : viewport()->setMaximumSize(800, 1200); break;
	}
	updateGeometry();
}

void GraphicsView::setupViewport ( QWidget * viewport ) {

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

    QGridLayout* ml = new QGridLayout(centralwidget);
    ml->setRowStretch(0, 1);
    ml->setMargin(0);
    ml->setHorizontalSpacing(0);
    ml->setVerticalSpacing(0);

    ml->setRowStretch(2, 1);
    ml->setColumnStretch(0, 1);
    ml->setColumnStretch(2, 1);
    ml->addWidget(new QWidget(), 0, 1);
    ml->addWidget(new QWidget(), 1, 0);
    ml->addWidget(new QWidget(), 1, 2);
    ml->addWidget(new QWidget(), 2, 1);
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
    graphicsWidget->setObjectName(QStringLiteral("centralwidget"));

    QWidget* topHeader = new QWidget(graphicsWidget);
    topHeader->setFixedHeight(30);
    topHeader->setPalette(green);
    topHeader->setAutoFillBackground(true);
    topHeader->setMaximumWidth(430);

    QWidget* leftHeader = new QWidget(graphicsWidget);
    leftHeader->setFixedWidth(30);
    leftHeader->setPalette(green);
    leftHeader->setAutoFillBackground(true);
    leftHeader->setMaximumHeight(630);

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

    QComboBox* zoomWidget = new QComboBox();
    zoomWidget->addItem("25%");
    zoomWidget->addItem("50%");
    zoomWidget->addItem("100%");
    zoomWidget->addItem("200%");
    toolBar->addWidget(zoomWidget);
    QObject::connect(zoomWidget, SIGNAL(currentIndexChanged(int)), view, SLOT(setZoom(int)));

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
    scene->setSceneRect(0, 0, 400, 600);
    view->viewport()->setMaximumSize(400, 600);

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


MainWindow::~MainWindow() {
    delete scene;
}
