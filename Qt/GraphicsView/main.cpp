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
#include <QApplication>
#include <QGraphicsDropShadowEffect>
#include <QVector2D>

#include "GraphicsSheet.h"
#include "ScrollAreaLayout.h"
#include "RectItem.h"
#include "LineItem.h"
#include "TextItem.h"
#include "EditFrameInteractor.h"

#include "LabelledComboBox.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent) {

    setObjectName(QStringLiteral("MainWindow"));
    resize(1024, 768);

    graphicsSheet = new GraphicsSheet(this);
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(graphicsSheet);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0xa0, 0xa0, 0xa0));
    graphicsSheet->setGraphicsEffect(shadow);

    graphicsSheet->setScaleBackground(QColor(0xFF, 0xFF, 0xF8));

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

    QAction* actionRotate = new QAction("Rotate", this);
    toolBar->addAction(actionRotate);
    QObject::connect(actionRotate, SIGNAL(triggered(bool)),
                     this, SLOT(rotateItem()));

    QAction* actionResize = new QAction("ResizeHandle", this);
    toolBar->addAction(actionResize);
    QObject::connect(actionResize, SIGNAL(triggered(bool)),
                     this, SLOT(resizeItem()));

    zoomWidget->getComboBox()->setCurrentIndex(2);
    sizeWidget->getComboBox()->setCurrentIndex(3);
    scaleWidget->getComboBox()->setCurrentIndex(2);
    checkbox->setChecked(true);

/*****************************************************************************/

    QGraphicsRectItem* item = new RectItem(QRectF(10, 10, 50, 50));
    item->setPen(QPen(Qt::red, 0));
    item->setBrush(Qt::cyan);
    graphicsSheet->scene()->addItem(item);

    item = new RectItem(QRectF(0, 0, 5, 5));
    item->setPen(QPen(Qt::blue, 0));
    item->setBrush(Qt::lightGray);
    graphicsSheet->scene()->addItem(item);

    item = new RectItem(QRectF(225, 295, 5, 5));
    item->setPen(QPen(Qt::green, 0));
    item->setBrush(Qt::lightGray);
    graphicsSheet->scene()->addItem(item);

    item1 = new RectItem(QRectF(125, 100, 200, 100));
    QPointF center2 = QPointF(item1->rect().width() / 2, item1->rect().height() / 2);
    item1->setTransformOriginPoint(center2);
    item1->setRotation(30);
    item1->setPen(QPen(Qt::magenta, 0));
    item1->setBrush(Qt::lightGray);
    graphicsSheet->scene()->addItem(item1);

    LineItem* item2 = new LineItem(QPointF(30, 30), QPointF(100, 200));
    graphicsSheet->scene()->addItem(item2);

    TextItem* item3 = new TextItem(QPoint(200, 50)); // , 100, 50));
    item3->setInternalFont(QFont("Arial", 24, 0, false));
    item3->setPen(QPen(Qt::darkRed, 2));
    item3->setBrush(QColor(255, 255, 240));
    item3->setText("Hg - Hello World");
    item3->setInternalDefaultTextColor(Qt::red);
    graphicsSheet->scene()->addItem(item3);

    LineItem* li1 = new LineItem(QPointF(0, 100), QPointF(100, 100));
    li1->setPen(QPen(Qt::red, 0));
    LineItem* li2 = new LineItem(QPointF(0, 104.23), QPointF(100, 104.23));
    li2->setPen(QPen(Qt::red, 0));
    graphicsSheet->scene()->addItem(li1);
    graphicsSheet->scene()->addItem(li2);

    graphicsSheet->setInteractor(new EditFrameInteractor());
}

#if 0
qreal calculateDistance(const QVector2D& v, const QVector2D& w, const QVector2D& p) {
    // Return minimum distance between line segment vw and point p
      const float l2 = length_squared(v, w);  // i.e. |w-v|^2 -  avoid a sqrt
      if (l2 == 0.0) return distance(p, v);   // v == w case

      // Consider the line extending the segment, parameterized as v + t (w - v).
      // We find projection of point p onto the line.
      // It falls where t = [(p-v) . (w-v)] / |w-v|^2
      const float t = dot(p - v, w - v) / l2;
      if (t < 0.0) return distance(p, v);       // Beyond the 'v' end of the segment
      else if (t > 1.0) return distance(p, w);  // Beyond the 'w' end of the segment
      const vec2 projection = v + t * (w - v);  // Projection falls on the segment

      return distance(p, projection);
}
#endif


// dist_Point_to_Segment(): get the distance of a point to a segment
//     Input:  a Point P and a Segment S (in any dimension)
//     Return: the shortest distance from P to S
static qreal dist_Point_to_Segment(const QPointF& P, const QPointF& P0, const QPointF& P1) {

     QVector2D v(P1 - P0);
     QVector2D w(P - P0);

     qreal c1 = QVector2D::dotProduct(w, v);
     if ( c1 <= 0 )
          return QLineF(P, P0).length();


     qreal c2 = QVector2D::dotProduct(v,v);
     if ( c2 <= c1 )
          return QLineF(P, P1).length();

     qreal b = c1 / c2;

     QPointF Pb(P0 + (b * v).toPointF());

     return QLineF(P, Pb).length();
}


void MainWindow::printInfo() {
    qDebug() << "\nLAYOUT INFO:\n====================";
    qDebug() << "   QGraphicsSheet: " << graphicsSheet->rect();
    qDebug() << "   viewport: " << graphicsSheet->viewport()->rect();

    QPointF p1(10, 10);
    QPointF p2(20, 20);
    QPointF p(40, 20);
    qreal dist = dist_Point_to_Segment(p, p1, p2);
    qDebug() << dist;
}


void MainWindow::rotateItem() {

}


void MainWindow::resizeItem() {
for (int i = 0;  i < 10;  i++) {
    QGraphicsRectItem* theFrame = item1;
    QPointF P1(100 - (7.0 * i)/3.0, 50 -(5.0*i)/3.0);

    graphicsSheet->addPoint("P1", P1);
    graphicsSheet->addPoint("pos", theFrame->pos());

    QPointF P2i = QPointF(theFrame->rect().width(), theFrame->rect().height());
    QPointF P2 = theFrame->mapToScene(P2i);

    qDebug() << "P1:" << P1;
    qDebug() << "P2:" << P2;
    graphicsSheet->addPoint("P2", P2);

    QTransform t;
    t.rotate(-theFrame->rotation());
    t.translate(-P1.x(), -P1.y());

    QPointF P2t = t.map(P2);

//    qDebug() << "P2t:" << P2t;  // OK

/*************************/
    QPointF P3(P2t.x()/2, P2t.y()/2);
 //   qDebug() << "P3:" << P3;

    QTransform t2;
    t2.translate(P1.x(), P1.y());
    t2.rotate(theFrame->rotation());
    QPointF P3t = t2.map(P3);
////    qDebug() << "P3t:" << P3t;      // OK

    graphicsSheet->addPoint("P3t", P3t);

    QTransform t3;
    t3.translate(P3t.x(), P3t.y());
    t3.rotate(-theFrame->rotation());
    t3.translate(-(P3t.x()), -(P3t.y()));

    QPointF newPos = t3.map(P1);
    QSizeF newSize = QSizeF(P2t.x(), P2t.y());

    graphicsSheet->addPoint("newPos", newPos);

    RectItem* item1 = new RectItem(QRectF(newPos.x(), newPos.y(), newSize.width(), newSize.height()));
    qreal angle = 30;
    QPointF center2 = QPointF(item1->rect().width() / 2, item1->rect().height() / 2);
    item1->setTransformOriginPoint(center2);
    item1->setRotation(angle);
    graphicsSheet->scene()->addItem(item1);

}
}



MainWindow::~MainWindow() {
}


int main(int argc, char ** argv) {

    QApplication app( argc, argv );

    MainWindow win(0);
    win.show(); 
    app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );

    return app.exec();
}
