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


class ScrollArea : public QScrollArea {
public:
    ScrollArea(QWidget* parent) : QScrollArea(parent) {
    }


    virtual QSize sizeHint() const {
        QSize result = widget()->sizeHint(); //viewport()->sizeHint();
        qDebug() << "ScrollArea::sizeHint(): " << result;
        return result;
    }

};



class CentralWidget : public QWidget {
    QScrollArea* theChild;

public:
    CentralWidget(QWidget* parent) : QWidget(parent) {
        QPalette pal;
        QBrush brush(Qt::cyan);
        brush.setStyle(Qt::SolidPattern);
        pal.setBrush(QPalette::Active, QPalette::Window, brush);
        setPalette(pal);
        setAutoFillBackground(true);
    }


    void setWidget(QScrollArea* wd) {
        theChild = wd;
    }

    void resizeEvent ( QResizeEvent * event ) {
        QWidget::resizeEvent(event);

        //qDebug() << "Resize:" << geometry();
        //qDebug() << "sizeHint:" << theChild->sizeHint();

        int wid = theChild->sizeHint().width();
        int h = theChild->sizeHint().height();

        int xpos = 0;
        int ypos = 0;
        if (width() < wid &&
            height() < h) {

            // both scrollbars required
            wid = width();
            h = height();

        } else if (width() < wid) {
            // only horizontal scroll bar required

            h += theChild->horizontalScrollBar()->height();

            wid = width();

            // the new y position is based on the exended height (including scroll bars)
            // this leads to a lesser smoothly transition from no scroll bars to scroll bars,
            // but avoids an asymmetry once the scroll bars are shown.
            ypos = (height() - h) / 2;

            if (height() < h) { // again both required
                xpos = 0;
                ypos = 0;
                h = height();
            }

        } else if (height() < h) {
            // only vertical scroll bar required
            wid += theChild->verticalScrollBar()->width();

            h = height();

            // the new x position is based on the extended width (including scroll bars)
            // this leads to a lesser smoothly transition from no scroll bars to scroll bars,
            // but avoids an asymmetry once the scroll bars are shown.
            xpos = (width() - wid) / 2;

            if (width() < wid) {  // again both required
                xpos = 0;
                ypos = 0;
                wid = width();
            }
        } else {
            xpos = (width() - wid) / 2;
            ypos = (height() - h) / 2;
        }

        //qDebug() << "new geom:" << xpos <<"," << ypos << "," << wid << "," << h;

        int frameWidth = 0; // !!!!
        theChild->setGeometry(xpos, ypos, wid + frameWidth, h + frameWidth);
    }
};

ViewportWidget::ViewportWidget(QWidget* parent) : QWidget(parent) {
    setAutoFillBackground(true);

    QPalette pal = palette();
    pal.setBrush(QPalette::Base, Qt::yellow);
    pal.setBrush(QPalette::Window, Qt::yellow);
    setPalette(pal);
}


QSize ViewportWidget::sizeHint() const {
    return QSize(320, 200);
}


void ViewportWidget::paintEvent ( QPaintEvent * event ) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);



/*
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
    p.drawText(textBox2, unit);*/

}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent) {

    setObjectName(QStringLiteral("MainWindow"));
    resize(1024, 768);

    CentralWidget* centralWidget = new CentralWidget(this);

    QWidget* viewPort = new ViewportWidget();
    viewPort->setGeometry(0, 0, viewPort->sizeHint().width(), viewPort->sizeHint().height());
    viewPort->setMinimumSize(viewPort->sizeHint());

    QScrollArea* scrollArea = new ScrollArea(centralWidget);

    // do not draw a frame around the scroll area - otherwise, we need
    // to add the frame width * 2 to the geometry in order to hide the scrollbars!
    scrollArea->setFrameStyle(0);
    scrollArea->setWidget(viewPort);
    // scrollArea->setViewport(viewPort);

    centralWidget->setWidget(scrollArea);

    scrollArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded);

    setCentralWidget(centralWidget);
}


void MainWindow::printInfo() {
	qDebug() << "\nLAYOUT INFO:\n====================";
}


MainWindow::~MainWindow() {
}
