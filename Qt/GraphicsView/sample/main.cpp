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
#include <QDebug>

#include "GraphicsSheet.h"
#include "GraphicsScene.h"
#include "ScrollAreaLayout.h"
#include "EditItemInteractor.h"
#include "NewItemInteractor.h"

#include "Snapper.h"

#include "LabelledComboBox.h"
#include "MainWindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    selectInteractor(0), newLineItemInteractor(0),
    newRectItemInteractor(0), newTextItemInteractor(0) {

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

    QCheckBox* checkbox = new QCheckBox("Landscape", toolBar);
    toolBar->addWidget(checkbox);
    QObject::connect(checkbox, SIGNAL(stateChanged(int)),
                     graphicsSheet, SLOT(setDirection(int)));

    QIcon icon;
    icon.addFile(QStringLiteral(":/Icons/file-load.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionLoad = new QAction(icon, "Load drawing ...", this);
    toolBar->addAction(actionLoad);
    QObject::connect(actionLoad, SIGNAL(triggered()), this, SLOT(doActionLoad()));

    QIcon icon2;
    icon2.addFile(QStringLiteral(":/Icons/file-save.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionSave = new QAction(icon2, "Save drawing", this);
    toolBar->addAction(actionSave);
    QObject::connect(actionSave, SIGNAL(triggered()), this, SLOT(doActionSave()));

    QIcon icon3;
    icon3.addFile(QStringLiteral(":/Icons/file-save-as.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionSaveAs = new QAction(icon3, "Save drawing as ...", this);
    toolBar->addAction(actionSaveAs);
    QObject::connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(doActionSaveAs()));

    QIcon icon4;
    icon4.addFile(QStringLiteral(":/Icons/object-select.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionSelect = new QAction(icon4, "Edit object", this);
    actionSelect->setCheckable(true);
    toolBar->addAction(actionSelect);
    QObject::connect(actionSelect, SIGNAL(triggered()), this, SLOT(doActionSelect()));

    QIcon icon5;
    icon5.addFile(QStringLiteral(":/Icons/object-newline.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionNewLineItem = new QAction(icon5, "New line", this);
    actionNewLineItem->setCheckable(true);
    toolBar->addAction(actionNewLineItem);
    QObject::connect(actionNewLineItem, SIGNAL(triggered()), this, SLOT(doActionNewLineItem()));

    QIcon icon6;
    icon6.addFile(QStringLiteral(":/Icons/object-newrect.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionNewRectItem = new QAction(icon6, "New rectangle", this);
    actionNewRectItem->setCheckable(true);
    toolBar->addAction(actionNewRectItem);
    QObject::connect(actionNewRectItem, SIGNAL(triggered()), this, SLOT(doActionNewRectItem()));

    QIcon icon7;
    icon7.addFile(QStringLiteral(":/Icons/object-newtext.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionNewTextItem = new QAction(icon7, "New text rectangle", this);
    actionNewTextItem->setCheckable(true);
    toolBar->addAction(actionNewTextItem);
    QObject::connect(actionNewTextItem, SIGNAL(triggered()), this, SLOT(doActionNewTextItem()));

    QIcon icon8;
    icon8.addFile(QStringLiteral(":/Icons/object-newcircle.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionNewCircleItem = new QAction(icon8, "New circle", this);
    actionNewCircleItem->setCheckable(true);
    toolBar->addAction(actionNewCircleItem);
    QObject::connect(actionNewCircleItem, SIGNAL(triggered()), this, SLOT(doActionNewCircleItem()));

    QIcon icon9;
    icon9.addFile(QStringLiteral(":/Icons/object-newellipse.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionNewEllipseItem = new QAction(icon9, "New ellipse", this);
    actionNewEllipseItem->setCheckable(true);
    toolBar->addAction(actionNewEllipseItem);
    QObject::connect(actionNewEllipseItem, SIGNAL(triggered()), this, SLOT(doActionNewEllipseItem()));

    QIcon icon10;
    icon10.addFile(QStringLiteral(":/Icons/object-newbezier.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionNewBezierItem = new QAction(icon10, "New bezier curve", this);
    actionNewBezierItem->setCheckable(true);
    toolBar->addAction(actionNewBezierItem);
    QObject::connect(actionNewBezierItem, SIGNAL(triggered()), this, SLOT(doActionNewBezierItem()));

    QIcon icon11;
    icon11.addFile(QStringLiteral(":/Icons/object-delete.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionDeleteItem = new QAction(icon11, "Delete selected objects", this);
    toolBar->addAction(actionDeleteItem);
    QObject::connect(actionDeleteItem, SIGNAL(triggered()), graphicsSheet, SLOT(deleteSelectedItems()));

    QIcon icon12;
    icon12.addFile(QStringLiteral(":/Icons/edit-redo.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionRedo = new QAction(icon12, "Redo last undone action", this);
    toolBar->addAction(actionRedo);
    QObject::connect(actionRedo, SIGNAL(triggered()), this, SLOT(doActionRedo()));

    QIcon icon13;
    icon13.addFile(QStringLiteral(":/Icons/edit-undo.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionUndo = new QAction(icon13, "Undo last action", this);
    toolBar->addAction(actionUndo);
    QObject::connect(actionUndo, SIGNAL(triggered()), this, SLOT(doActionUndo()));

    QActionGroup* actionGroup = new QActionGroup(this);
    actionGroup->addAction(actionSelect);
    actionGroup->addAction(actionNewLineItem);
    actionGroup->addAction(actionNewRectItem);
    actionGroup->addAction(actionNewTextItem);
    actionGroup->addAction(actionNewCircleItem);
    actionGroup->addAction(actionNewEllipseItem);
    actionGroup->addAction(actionNewBezierItem);
    actionSelect->setChecked(true);

#if 0
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
#endif

    zoomWidget->getComboBox()->setCurrentIndex(2);
    sizeWidget->getComboBox()->setCurrentIndex(3);
    scaleWidget->getComboBox()->setCurrentIndex(2);
    checkbox->setChecked(true);

/*****************************************************************************/

    GraphicsScene* scene = dynamic_cast<GraphicsScene*>(graphicsSheet->scene());
    scene->loadFromFile("sample.drw");

    selectInteractor = new EditItemInteractor();
// TODO: get all registered items from the factory!
#if 0
    newLineItemInteractor = new NewItemInteractor(LineItem::create, 1); // LineItem::P2Handle);
    QObject::connect(newLineItemInteractor, SIGNAL(editDone()), this, SLOT(toggleActionSelect()));
    newRectItemInteractor = new NewItemInteractor(RectItem::create, 1); // RectItem::BottomRightHandle);
    QObject::connect(newRectItemInteractor, SIGNAL(editDone()), this, SLOT(toggleActionSelect()));
    newTextItemInteractor = new NewItemInteractor(TextItem::create, 1); // TextItem::BottomRightHandle);
    QObject::connect(newTextItemInteractor, SIGNAL(editDone()), this, SLOT(toggleActionSelect()));
    newCircleItemInteractor = new NewItemInteractor(CircleItem::create, 1); // CircleItem::RadHandle);
    QObject::connect(newCircleItemInteractor, SIGNAL(editDone()), this, SLOT(toggleActionSelect()));
    newEllipseItemInteractor = new NewItemInteractor(EllipseItem::create, 1); // EllipseItem::BottomRightHandle);
    QObject::connect(newEllipseItemInteractor, SIGNAL(editDone()), this, SLOT(toggleActionSelect()));
    newBezierItemInteractor = new NewItemInteractor(BezierItem::create, 1); // BezierItem::P2Handle);
    QObject::connect(newBezierItemInteractor, SIGNAL(editDone()), this, SLOT(toggleActionSelect()));
#endif

    graphicsSheet->setInteractor(selectInteractor);
    graphicsSheet->setSnapper(new EdgeSnapper(new GridSnapper()));
}


#if 0

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
#endif


void MainWindow::doActionLoad(){
    GraphicsScene* scene = dynamic_cast<GraphicsScene*>(graphicsSheet->scene());
    scene->loadFromFile("sample.drw");
}


void MainWindow::doActionSave(){
    GraphicsScene* scene = dynamic_cast<GraphicsScene*>(graphicsSheet->scene());
    scene->saveToFile("sample.drw");
}


void MainWindow::doActionSaveAs(){
    qDebug() << "SAVE AS";
}

void MainWindow::toggleActionSelect() {
    actionSelect->setChecked(true);
    doActionSelect();
}

void MainWindow::doActionSelect(){
    graphicsSheet->setInteractor(selectInteractor);
}

void MainWindow::doActionNewLineItem(){
    graphicsSheet->setInteractor(newLineItemInteractor);
}

void MainWindow::doActionNewRectItem(){
    graphicsSheet->setInteractor(newRectItemInteractor);
}

void MainWindow::doActionNewTextItem(){
    graphicsSheet->setInteractor(newTextItemInteractor);
}

void MainWindow::doActionNewCircleItem(){
    graphicsSheet->setInteractor(newCircleItemInteractor);
}

void MainWindow::doActionNewEllipseItem(){
    graphicsSheet->setInteractor(newEllipseItemInteractor);
}

void MainWindow::doActionNewBezierItem(){
    graphicsSheet->setInteractor(newBezierItemInteractor);
}

void MainWindow::doActionRedo(){
    qDebug() << "REDO";
}

void MainWindow::doActionUndo(){
    qDebug() << "UNDO";
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
