#include <QMouseEvent>

#include "GraphicsSheet.h"
#include "TextItem.h"
#include "NewTextItemInteractor.h"
#include "Log.h"

NewTextItemInteractor::NewTextItemInteractor()  {
}


NewTextItemInteractor::~NewTextItemInteractor() {
}


void NewTextItemInteractor::mousePressEvent ( QMouseEvent * event ) {
	if (event->button() != Qt::LeftButton) {
		return;
	}

	// Log::log(Log::DEBUG, "NewTextItemInteractor") << "mousePressEvent";

	QPointF scenePos = theView->mapToScene(event->pos());
//    QPoint pos = QPoint((int) floor(scenePos.x()),
//                        (int) floor(scenePos.y()));
//	KollageGraphicsScene* theScene = dynamic_cast<KollageGraphicsScene*>(theView->scene());

	TextItem* newItem = new TextItem(scenePos);
    theView->scene()->clearSelection();
    theItem = newItem;
    theView->scene()->addItem(newItem);
    theItem->setItemSelected(true);
    newItem->setText("Text");
    // theFrame->setZValue(theScene->zOrder++);
	editHandle = RectItem::BottomRightHandle;
	offset = QSize(0,0);
}

void NewTextItemInteractor::mouseReleaseEvent ( QMouseEvent* ) {
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
