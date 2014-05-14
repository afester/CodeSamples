/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

#include <QDebug>
#include <QLayout>
#include <QLabel>
#include <QKeyEvent>
#include <QPainter>

#include "WidgetSample.h"


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


CustomWidget::CustomWidget(QWidget* parent) : QWidget(parent) {
    setAutoFillBackground(true);

    QPalette pal = palette();
    pal.setBrush(QPalette::Base, Qt::cyan);
    pal.setBrush(QPalette::Window, Qt::cyan);
    setPalette(pal);
}


QSize CustomWidget::sizeHint() const {
    return QWidget::sizeHint();
    // return QSize(320, 200);
}


void CustomWidget::paintEvent ( QPaintEvent * event ) {
    QPainter p(this);
    // p.setRenderHint(QPainter::Antialiasing, true);

    p.setPen(QPen(Qt::red, 0, Qt::DashLine));

    // aliased rectangle is drawn to the right and below the defined points!
    QRect aliasedRectangle = QRect(0, 0, width()-1, height()-1);
    qDebug() << aliasedRectangle;
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
    resize(400, 300);

    QWidget* centralWidget = new QWidget;

    QWidget* viewport = new QWidget(this);
    viewport->setAutoFillBackground(true);
    QPalette pal = viewport->palette();
    pal.setBrush(QPalette::Base, Qt::white);
    pal.setBrush(QPalette::Window, Qt::white);
    viewport->setPalette(pal);
    viewport->setFixedSize(100, 100);

    QLayout* layout = new CenterLayout(centralWidget, viewport);
    layout->setObjectName("CenterLayout1");
    centralWidget->setLayout(layout);
#if 0
    QLabel* imageLabel = new QLabel(viewport);
    QImage image("sample.png");
    imageLabel->setPixmap(QPixmap::fromImage(image));
    childWidget = imageLabel;
#endif
    childWidget = new CustomWidget(viewport);
    childWidget->resize(320, 200);

    setCentralWidget(centralWidget);
}


MainWindow::~MainWindow() {
}


void MainWindow::keyPressEvent ( QKeyEvent * event ) {
    switch(event->key()) {
        case Qt::Key_Left :
                childWidget->move(childWidget->x() + 20, childWidget->y());
                break;

        case Qt::Key_Up :
                childWidget->move(childWidget->x(), childWidget->y() + 20);
                break;

        case Qt::Key_Right :
                childWidget->move(childWidget->x() - 20, childWidget->y());
                break;

        case Qt::Key_Down :
                childWidget->move(childWidget->x(), childWidget->y() - 20);
                break;
    }
}
