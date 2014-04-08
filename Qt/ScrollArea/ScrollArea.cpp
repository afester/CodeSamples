/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

#include <iostream>
#include <typeinfo>

#include <QDebug>
#include <QPainter>
#include <QScrollArea>
#include <QScrollBar>
#include <QLayout>

#include "ScrollArea.h"


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



ViewportWidget::ViewportWidget(QWidget* parent) : QWidget(parent) {
    setAutoFillBackground(true);

    QPalette pal = palette();
    pal.setBrush(QPalette::Base, Qt::white);
    pal.setBrush(QPalette::Window, Qt::white);
    setPalette(pal);
}


QSize ViewportWidget::sizeHint() const {
    return QSize(320, 200);
}


void ViewportWidget::paintEvent ( QPaintEvent * event ) {
    QPainter p(this);
    p.setPen(QPen(Qt::red, 0, Qt::DashLine));

    // aliased rectangle is drawn to the right and below the defined points!
    QRect aliasedRectangle = QRect(x(), y(), width()-1, height()-1);
    p.drawRect(aliasedRectangle);
}


ScrollAreaLayout::ScrollAreaLayout() : theItem(0) {
}


QSize ScrollAreaLayout::sizeHint() const {
    if (theItem != 0) {
        return theItem->sizeHint();
    }
    return QSize(-1,-1);
}


void ScrollAreaLayout::addItem(QLayoutItem *item) {
    if (theItem == 0) {
        theItem = item;
    }
}


QLayoutItem *ScrollAreaLayout::itemAt(int index) const {
    if (index == 0) {
        return theItem;
    }

    return 0;
}


QLayoutItem *ScrollAreaLayout::takeAt(int index) {
    QLayoutItem* result = 0;
    if (index == 0) {
        result = theItem;
        theItem = 0;
    }
    return result;
}


int ScrollAreaLayout::count() const {
    if (theItem == 0) {
        return 0;
    }
    return 1;
}


void ScrollAreaLayout::setGeometry ( const QRect & r )  {
    QScrollArea* theChild = dynamic_cast<QScrollArea*>(theItem->widget());

    //qDebug() << "--------------------------------------";
    //qDebug() << "  Layout geometry:" << r;
    //qDebug() << "  Scroll bar width: " << theChild->verticalScrollBar()->width();

    int wid = theChild->sizeHint().width(); // frame width is considered!
    int h = theChild->sizeHint().height();  // frame height is considered!
    int xpos = 0;
    int ypos = 0;

    if (r.width() < wid &&
        r.height() < h) {

        // both scrollbars required
        wid = r.width();
        h = r.height();

    } else if (r.width() < wid) {
        // only horizontal scroll bar required

        h += theChild->horizontalScrollBar()->sizeHint().height();

        wid = r.width();

        // the new y position is based on the extended height (including scroll bars)
        // this leads to a lesser smoothly transition from no scroll bars to scroll bars,
        // but avoids an asymmetry once the scroll bars are shown.
        ypos = (r.height() - h) / 2;

        if (r.height() < h) { // again both required
            xpos = 0;
            ypos = 0;
            h = r.height();
        }

    } else if (r.height() < h) {
        // only vertical scroll bar required
        wid += theChild->verticalScrollBar()->sizeHint().width();

        h = r.height();

        // the new x position is based on the extended width (including scroll bars)
        // this leads to a lesser smoothly transition from no scroll bars to scroll bars,
        // but avoids an asymmetry once the scroll bars are shown.
        xpos = (r.width() - wid) / 2;

        if (r.width() < wid) {  // again both required
            xpos = 0;
            ypos = 0;
            wid = r.width();
        }
    } else {
        xpos = (r.width() - wid) / 2;
        ypos = (r.height() - h) / 2;
    }

    int frameWidth = 0; // !!!!
    QRect newRect = QRect(xpos, ypos, wid + frameWidth, h + frameWidth);
    // qDebug() << "  Child geometry:" << newRect;

    theChild->setGeometry(newRect);
}



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent) {

    setObjectName(QStringLiteral("MainWindow"));
    resize(1024, 768);

    QWidget* viewPort = new ViewportWidget();
    viewPort->setGeometry(0, 0, viewPort->sizeHint().width(), viewPort->sizeHint().height());
    viewPort->setMinimumSize(viewPort->sizeHint());

    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setObjectName("ScrollArea1");
    scrollArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded);
    scrollArea->setWidget(viewPort);

    QLayout* layout = new ScrollAreaLayout();
    layout->addWidget(scrollArea);

    QWidget* centralWidget = new ColoredWidget(QColor(255, 255, 240), this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}


void MainWindow::printInfo() {
	qDebug() << "\nLAYOUT INFO:\n====================";
}


MainWindow::~MainWindow() {
}
