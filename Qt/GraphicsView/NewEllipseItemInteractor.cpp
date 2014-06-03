#include <QMouseEvent>

#include "GraphicsSheet.h"
#include "EllipseItem.h"
#include "NewEllipseItemInteractor.h"
#include "Log.h"

NewEllipseItemInteractor::NewEllipseItemInteractor()  {
}


NewEllipseItemInteractor::~NewEllipseItemInteractor() {
}


void NewEllipseItemInteractor::mousePressEvent ( QMouseEvent * event ) {
	if (event->button() != Qt::LeftButton) {
		return;
	}

	QPointF scenePos = theView->mapToScene(event->pos());

	EllipseItem* newItem = new EllipseItem(scenePos);
    theView->scene()->clearSelection();
    theItem = newItem;
    theView->scene()->addItem(newItem);
    theItem->setItemSelected(true);
    // theFrame->setZValue(theScene->zOrder++);
	editHandle = EllipseItem::BottomRightHandle;
	offset = QPointF(0,0);
}


void NewEllipseItemInteractor::mouseReleaseEvent ( QMouseEvent* ) {
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
