/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

#include "Canvas.h"

#include <QLabel>
#include <QPainter>

Canvas::Canvas(QWidget* parent, int width, int height) : QWidget(parent), pixmap(QPixmap(width, height)) {

    pixmap.fill(Qt::white);

    thePainter = new QPainter(&pixmap);
    thePainter->setRenderHints(QPainter::Antialiasing, true);
    thePainter->setPen(Qt::black);
}

void Canvas::paintEvent(QPaintEvent * event) {
    QPainter p(this);
    p.drawPixmap(0, 0, pixmap);
}

void Canvas::drawLine(int x1, int y1, int x2, int y2) {
    thePainter->drawLine(x1, y1, x2, y2);
    update();
}

void Canvas::drawEllipse(int x, int y, int w, int h) {
    thePainter->drawEllipse(x, y, w, h);
    update();
}
