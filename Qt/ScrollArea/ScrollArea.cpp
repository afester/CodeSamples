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
        QSize result = viewport()->sizeHint();
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

        qDebug() << "Resize:" << geometry();
        qDebug() << "sizeHint:" << theChild->sizeHint();

        int xpos = (width() - theChild->width()) / 2;
        int ypos = (height() - theChild->height()) / 2;
        theChild->setGeometry(xpos, ypos,
                              theChild->sizeHint().width(), theChild->sizeHint().height());
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

    QScrollArea* scrollArea = new ScrollArea(centralWidget);
    scrollArea->setViewport(viewPort);

    centralWidget->setWidget(scrollArea);

    scrollArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOn);

    setCentralWidget(centralWidget);
}


void MainWindow::printInfo() {
	qDebug() << "\nLAYOUT INFO:\n====================";
}


MainWindow::~MainWindow() {
}
