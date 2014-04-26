/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

#include <QPainter>

#include "ScaleWidget.h"
#include "GraphicsSheet.h"

ScaleWidget::ScaleWidget(QWidget* parent, GraphicsSheet* view, Direction dir) :
            QWidget(parent),
            theScale(1.0), offset(0), positionIndicator(-1),
            smallTicksSize(4), mediumTicksSize(6), largeTicksSize(8),
            theView(view), direction(dir)  {
    setAutoFillBackground(true);
    setFont(QFont("Sans", 6));  // default font

    QPalette pal = palette();
    pal.setBrush(QPalette::Base, Qt::white);
    pal.setBrush(QPalette::Window, Qt::white);
    pal.setColor(QPalette::Foreground, QColor(0x80, 0x80, 0x80));
    setPalette(pal);
}


void ScaleWidget::setScale(qreal scale) {
    theScale = scale;
}


void ScaleWidget::setOffset(int value) {
    offset = value;
    repaint();
}


qreal ScaleWidget::snapToTick(int pos) {
    qreal scale = theView->transform().m22() / theScale;

    int tick = qRound(pos / scale);

    float result = tick / theScale;
    return result;
}


void ScaleWidget::setPos(qreal pos) {
    positionIndicator = pos;
    repaint();
}


void ScaleWidget::paintEvent ( QPaintEvent * event ) {
    QPainter p(this);

    QFontMetrics fm = fontMetrics();


    if (direction == Vertical) {
        p.setPen(Qt::lightGray);
        p.drawLine(width() - 1, 0, width() - 1, height());

        p.setPen(Qt::darkGray);
        p.setRenderHint(QPainter::Antialiasing);
        p.drawLine(0, 0, 0, height());
        p.drawLine(1, 0, 1, height() - 2);

        qreal scale = theView->transform().m22() / theScale;
        QTextOption option;
        option.setAlignment(Qt::AlignRight);

        for (int y = 0;  y < (theView->viewport()->height() + offset ) / scale;  y++) {
            float ypos = y*scale - offset;

            if ( (y % 10) == 0) {
                p.drawLine(QPointF(20 - largeTicksSize, ypos),
                           QPointF(width() - 4, ypos));

                if (y > 0) {
                    QRectF textBox(0, ypos - fm.height(), 14, fm.height());
                    //p.drawRect(textBox);
                    p.drawText(textBox, QString::number(y / theScale), option);
                }
            } else if ( (y % 5) == 0) {
                p.drawLine(QPointF(20 - mediumTicksSize, ypos),
                           QPointF(width() - 4, ypos));
            } else {
                p.drawLine(QPointF(20 - smallTicksSize, ypos),
                           QPointF(width() - 4, ypos));
            }
        }

        // draw position indicator if enabled
        if (positionIndicator >= 0) {
            p.setPen(QPen(Qt::red, 0, Qt::DashLine));
            float ypos = positionIndicator * theView->transform().m22() - offset;
            p.drawLine(0, ypos, width(), ypos);
        }

    } else {
        p.setPen(Qt::lightGray);
        p.drawLine(0, height() - 1, width(), height() - 1);

        p.setPen(Qt::darkGray);
        p.setRenderHint(QPainter::Antialiasing);
        p.drawLine(0, 0, width(), 0);
        p.drawLine(0, 1, width() - 2, 1);

        qreal scale = theView->transform().m11() / theScale;
        QTextOption option;
        option.setAlignment(Qt::AlignRight);

        for (int x = 0;  x < (theView->viewport()->width() + offset ) / scale;  x++) {
            float xpos = x*scale - offset;

            if ( (x % 10) == 0) {
                p.drawLine(QPointF(xpos, 19 - largeTicksSize),
                           QPointF(xpos, height() - 4));

                if (x > 0) {
                    QRectF textBox(xpos - 20, 3,
                            //(x-9)*scale, 0, // 22 - largeTicksSize - fm.height(),
                                    18, fm.height());
                    //p.drawRect(textBox);
                    p.drawText(textBox, QString::number(x / theScale), option);
                }
            } else if ( (x % 5) == 0) {
                p.drawLine(QPointF(xpos, 19 - mediumTicksSize),
                           QPointF(xpos, height() - 4));
            } else {
                p.drawLine(QPointF(xpos, 19 - smallTicksSize),
                           QPointF(xpos, height() - 4));
            }
        }

        // draw position indicator if enabled
        if (positionIndicator >= 0) {
            p.setPen(QPen(Qt::red, 0, Qt::DashLine));
            float xpos = positionIndicator * theView->transform().m11() - offset;
            p.drawLine(xpos, 0, xpos, height());
        }
    }
}
