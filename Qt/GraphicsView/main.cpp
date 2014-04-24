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

#include "GraphicsSheet.h"
#include "ScrollAreaLayout.h"
#include "EditableItem.h"
#include "EditFrameInteractor.h"

#include "LabelledComboBox.h"

#if 0
GraphicsItem::GraphicsItem ( qreal x, qreal y, qreal width, qreal height, QGraphicsItem * parent) :
        QGraphicsRectItem(x, y, width, height, parent) {
}


void GraphicsItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) {
    GraphicsSheet* requestingView = dynamic_cast<GraphicsSheet*>(widget->parent());
    if (requestingView) {
        painter->setPen(QPen(Qt::black, 0));
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

#endif
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

    zoomWidget->getComboBox()->setCurrentIndex(2);
    sizeWidget->getComboBox()->setCurrentIndex(3);
    scaleWidget->getComboBox()->setCurrentIndex(2);
    checkbox->setChecked(true);

    // graphicsSheet->setColor(Qt::blue);  // A blue view
/*****************************************************************************/

    QGraphicsRectItem* item = new QGraphicsRectItem(0, 0, 50, 50);
    item->setPos(10, 10);
    item->setPen(QPen(Qt::red, 0));
    graphicsSheet->scene()->addItem(item);
    qDebug() << "RED RECT:" << item->x() << "/" << item->y();
    qDebug() << "RED BOUNDINGRECT:" << item->boundingRect();

    item = new QGraphicsRectItem(0, 0, 5, 5);
    graphicsSheet->scene()->addItem(item);

    item = new QGraphicsRectItem(225, 295, 5, 5);
    graphicsSheet->scene()->addItem(item);

    item = new EditableItem(QRectF(125, 100, 200, 100));
    graphicsSheet->scene()->addItem(item);

    graphicsSheet->setInteractor(new EditFrameInteractor());
}


void MainWindow::printInfo() {
    qDebug() << "\nLAYOUT INFO:\n====================";
    qDebug() << "   QGraphicsSheet: " << graphicsSheet->rect();
    qDebug() << "   viewport: " << graphicsSheet->viewport()->rect();
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
