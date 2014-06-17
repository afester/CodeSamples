/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

#include <QScrollArea>
#include <QScrollBar>

#include "ScrollAreaLayout.h"


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
    QAbstractScrollArea* theChild = dynamic_cast<QAbstractScrollArea*>(theItem->widget());

    //qDebug() << "--------------------------------------";
    // qDebug() << "  Layout geometry:" << r;
    //qDebug() << "  Scroll bar width: " << theChild->verticalScrollBar()->width();

    // get the widget which is scrolled by the scrollarea
    //QWidget* wdg = theChild->widget();
   // qDebug() << "Widget sizeHint():" << wdg->objectName() << " - "<< wdg->sizeHint();
    //qDebug() << "Widget size():" << wdg->objectName() << " - "<< wdg->size();

   // int fh = theChild->fontMetrics().height();
    //qDebug() << "Scroll area max size:" << QSize(36 * fh, 24 * fh);

    int wid = theChild->sizeHint().width(); // frame width is considered!
    int h = theChild->sizeHint().height();  // frame height is considered!
    int xpos = 0;
    int ypos = 0;

    if (r.width() < wid && r.height() < h) {

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
    //qDebug() << "  Child geometry:" << newRect;

    theChild->setGeometry(newRect);
}

