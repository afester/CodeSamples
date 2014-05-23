#include <QMouseEvent>

#include "GraphicsSheet.h"
#include "LineItem.h"
#include "NewLineItemInteractor.h"
#include "Log.h"

NewLineItemInteractor::NewLineItemInteractor()  {
}


NewLineItemInteractor::~NewLineItemInteractor() {
}


void NewLineItemInteractor::mousePressEvent ( QMouseEvent * event ) {
	if (event->button() != Qt::LeftButton) {
		return;
	}

	// Log::log(Log::DEBUG, "NewRectItemInteractor") << "mousePressEvent";

	QPointF scenePos = theView->mapToScene(event->pos());
//    QPoint pos = QPoint((int) floor(scenePos.x()),
//                        (int) floor(scenePos.y()));
//	KollageGraphicsScene* theScene = dynamic_cast<KollageGraphicsScene*>(theView->scene());

	LineItem* newItem = new LineItem(scenePos, scenePos);
    theView->scene()->clearSelection();
    theItem = newItem;
    theView->scene()->addItem(newItem);
    theItem->setItemSelected(true);
    // theFrame->setZValue(theScene->zOrder++);
	editHandle = LineItem::P2Handle;
	offset = QPointF(0,0);
}

void NewLineItemInteractor::mouseReleaseEvent ( QMouseEvent* ) {
	// Log::log(Log::DEBUG, "EditFrameInteractor") << "mouseReleaseEvent";

	if (theItem) {
#if 0
		KollageGraphicsScene* theScene = dynamic_cast<KollageGraphicsScene*>(theView->scene());

        QUndoCommand* undo = new NewItemCommand(theScene, theFrame);
        theScene->getUndoStack()->push(undo);
        theFrame->setSelected(true);

        emit editDone();
#endif
		theItem = 0;
	}
}
