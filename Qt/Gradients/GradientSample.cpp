/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

#include <QPainter>

#include "GradientSample.h"

CustomWidget::CustomWidget(QWidget* parent) : QWidget(parent) {
}

void CustomWidget::paintEvent ( QPaintEvent * event ) {
    QPainter p(this);

	p.drawText(20, 100, "Hello World");
	QLinearGradient lg(10, 10, 210, 210);
    lg.setColorAt(0, QColor(0, 0, 255, 255));
    lg.setColorAt(1, QColor(255, 255, 255, 0));
    p.fillRect(10, 10, 200, 200, lg);

	p.drawText(230, 100, "Hello World");
	QLinearGradient lg2(220, 10, 420, 210);
    lg2.setColorAt(0, QColor(0, 0, 255, 255));
    lg2.setColorAt(1, QColor(255, 255, 255, 255));
    p.fillRect(220, 10, 200, 200, lg2);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setObjectName(QStringLiteral("MainWindow"));
    resize(400, 300);

    QWidget* centralWidget = new CustomWidget(this);
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow() {
}
