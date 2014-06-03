#include <QMouseEvent>

#include "GraphicsSheet.h"
#include "CircleItem.h"
#include "NewCircleItemInteractor.h"
#include "Log.h"

NewCircleItemInteractor::NewCircleItemInteractor()  {
}


NewCircleItemInteractor::~NewCircleItemInteractor() {
}


void NewCircleItemInteractor::mousePressEvent ( QMouseEvent * event ) {
	if (event->button() != Qt::LeftButton) {
		return;
	}

	QPointF scenePos = theView->mapToScene(event->pos());

	CircleItem* newItem = new CircleItem(scenePos);
    theView->scene()->clearSelection();
    theItem = newItem;
    theView->scene()->addItem(newItem);
    theItem->setItemSelected(true);
    // theFrame->setZValue(theScene->zOrder++);
	editHandle = CircleItem::RadHandle;
	offset = QPointF(0,0);
}


void NewCircleItemInteractor::mouseReleaseEvent ( QMouseEvent* ) {
	// Log::log(Log::DEBUG, "EditFrameInteractor") << "mouseReleaseEvent";

	if (theItem) {
#if 0
		KollageGraphicsScene* theScene = dynamic_cast<KollageGraphicsScene*>(theView->scene());

        QUndoCommand* undo = new NewItemCommand(theScene, theFrame);
        theScene->getUndoStack()->push(undo);
        theFrame->setSelected(true);
#endif
		theItem = 0;
        emit editDone();
	}
}
