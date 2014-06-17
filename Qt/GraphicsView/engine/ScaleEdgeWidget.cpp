/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

#include <QPainter>

#include "ScaleEdgeWidget.h"


ScaleEdgeWidget::ScaleEdgeWidget(QWidget* parent) : QWidget(parent) {
    setAutoFillBackground(true);
    setFont(QFont("Sans", 6));  // default font

    QPalette pal = palette();
    pal.setBrush(QPalette::Base, Qt::white);
    pal.setBrush(QPalette::Window, Qt::white);
    setPalette(pal);
}


void ScaleEdgeWidget::setUnit(const QString& theUnit) {
    unit = theUnit;
}


void ScaleEdgeWidget::paintEvent ( QPaintEvent * event ) {
    Q_UNUSED(event);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

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
    p.drawText(textBox2, unit);
}
