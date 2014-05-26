
#include "Snapper.h"
#include "Log.h"
#include "GraphicsSheet.h"

Snapper::~Snapper() {
}


void Snapper::setView(GraphicsSheet* view) {
    theView = view;
}


QPointF UnitSnapper::snap(const QPointF& pos) {
    return QPointF(qRound(pos.x()), qRound(pos.y()));
}
