
#include <QDebug>
#include <QGraphicsItem>
#include <math.h>

#include "Snapper.h"
#include "Log.h"
#include "GraphicsSheet.h"
#include "InteractableItem.h"

Snapper::~Snapper() {
}


void Snapper::setView(GraphicsSheet* view) {
    theView = view;
}


QPointF NoSnapper::snap(const QPointF& pos) {
    return pos;
}


QPointF UnitSnapper::snap(const QPointF& pos) {
    return QPointF(qRound(pos.x()), qRound(pos.y()));
}


QPointF GridSnapper::snap(const QPointF& pos) {
    return QPointF( qRound(qRound(pos.x() * theView->getScale()) / theView->getScale()),
                    qRound(qRound(pos.y() * theView->getScale()) / theView->getScale()));
}


EdgeSnapper::EdgeSnapper(Snapper* fallback) : fallbackSnapper(fallback) {
}


void EdgeSnapper::setView(GraphicsSheet* view) {
    theView = view;

    // need to set the view for the fallback snapper also!
    fallbackSnapper->setView(view);
}


QPointF EdgeSnapper::snap(const QPointF& pos) {
    QGraphicsScene* scene = theView->scene();

    // TODO: probably should use the view to get the items so that
    // the same visible area is always used for item selection, independent
    // of view transformation
    QList<QGraphicsItem *> items =
            scene->items (QRectF(pos.x() - 4, pos.y() - 4, 8, 8),
                          Qt::IntersectsItemShape);

    qreal minDist = 4.0; // minimum distance
    QPointF result = pos;
    bool found = false;
    //qDebug() << "Items: " << items.size() - 1;
    foreach(QGraphicsItem* item, items) {
        InteractableItem* iItem =  dynamic_cast<InteractableItem*>(item);
        if (iItem && iItem != theView->getFocusItem()) {  // TODO: Better approach to filter out current item?
            QPointF edge = iItem->getNearestEdge(theView, pos);
            qreal dist =  sqrt(pow(pos.x() - edge.x(), 2) + pow(pos.y() - edge.y(), 2));

            if (dist < minDist) {
                found = true;
                minDist = dist;
                result = edge;
            }
        }
    }

    if (!found) {
        result = fallbackSnapper->snap(pos);
    }
    return result;
}
