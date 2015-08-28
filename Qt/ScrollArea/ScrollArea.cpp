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
#include <QLabel>

#include "ScrollArea.h"
#include "ScrollAreaLayout.h"


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


class UnboundedScrollArea : public QScrollArea {
    mutable QSize widgetSize;

public:
    UnboundedScrollArea(QWidget* parent = 0) : QScrollArea(parent) {
    }

    QSize sizeHint() const {
        int f = 2 * frameWidth();
        QSize sz(f, f);
        int h = fontMetrics().height();

        if (widget()) {
             if (!widgetSize.isValid())
                 widgetSize = widgetResizable() ? widget()->sizeHint() : widget()->size();
             sz += widgetSize;
         } else {
             sz += QSize(12 * h, 8 * h);
         }
         if (verticalScrollBarPolicy() == Qt::ScrollBarAlwaysOn)
             sz.setWidth(sz.width() + verticalScrollBar()->sizeHint().width());
         if (horizontalScrollBarPolicy() == Qt::ScrollBarAlwaysOn)
             sz.setHeight(sz.height() + horizontalScrollBar()->sizeHint().height());

         return sz; // .boundedTo(QSize(36 * h, 24 * h));
    }
};


CustomWidget::CustomWidget(QWidget* parent) : QWidget(parent) {
    setAutoFillBackground(true);

    QPalette pal = palette();
    pal.setBrush(QPalette::Base, Qt::white);
    pal.setBrush(QPalette::Window, Qt::white);
    setPalette(pal);
}


QSize CustomWidget::sizeHint() const {
    //return QWidget::sizeHint();
   return QSize(320, 200);
}


void CustomWidget::paintEvent ( QPaintEvent * event ) {
    QPainter p(this);
    // p.setRenderHint(QPainter::Antialiasing, true);

    p.setPen(QPen(Qt::red, 0, Qt::DashLine));

    // aliased rectangle is drawn to the right and below the defined points!
    QRect aliasedRectangle = QRect(0, 0, width()-1, height()-1);
    p.drawRect(aliasedRectangle);

    aliasedRectangle = QRect(1, 1, width()-3, height()-3);
    p.setPen(QPen(Qt::blue, 0, Qt::DashLine));
    p.drawRect(aliasedRectangle);

    QRect topLeft = QRect(0, 0, 10, 10);
    QRect bottomRight = QRect(width() - 11, height() - 11, 10, 10);
    p.setPen(Qt::black);
    p.drawRect(topLeft);
    p.drawRect(bottomRight);

    p.setPen(QPen(Qt::black, 0, Qt::DashLine));
    p.drawLine(0, 0, width() - 1, height() - 1);
    p.drawLine(0, height() - 1, width() - 1, 0);

}


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    setObjectName(QStringLiteral("MainWindow"));
    resize(800, 600);

#if 0
    QWidget* wdg = new CustomWidget();
    wdg->resize(640, 480);
#endif

    QLabel* imageLabel = new QLabel;
    QImage image("sample.png");
    imageLabel->setPixmap(QPixmap::fromImage(image));
    QWidget* wdg = imageLabel;

    QScrollArea* scrollArea = new UnboundedScrollArea(this);
    scrollArea->setObjectName("ScrollArea1");
    scrollArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded);
    scrollArea->setWidget(wdg);

    QLayout* layout = new ScrollAreaLayout();
    layout->addWidget(scrollArea);

    QWidget* centralWidget = new ColoredWidget(QColor(255, 255, 240), this);
    centralWidget->setLayout(layout);

    setCentralWidget(centralWidget);
}


MainWindow::~MainWindow() {
}
