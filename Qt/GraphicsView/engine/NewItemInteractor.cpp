#include <QMouseEvent>
#include <QGraphicsItem>

#include "GraphicsSheet.h"
#include "NewItemInteractor.h"

NewItemInteractor::NewItemInteractor(FACTORY_FUNCTION factory, AbstractEditHandle handle) :
itemFactory(factory) {
    editHandle = handle;
}


NewItemInteractor::~NewItemInteractor() {
}


void NewItemInteractor::mousePressEvent ( QMouseEvent * event ) {
	if (event->button() != Qt::LeftButton) {
		return;
	}

	QPointF scenePos = theView->mapToScene(event->pos());

	QGraphicsItem* newItem = itemFactory();
	newItem->setPos(scenePos);
    theView->scene()->clearSelection();
    theItem = dynamic_cast<InteractableItem*>(newItem);
    theView->scene()->addItem(newItem);
    theItem->setItemSelected(true);
    // theFrame->setZValue(theScene->zOrder++);
	offset = QPointF(0,0);
}


void NewItemInteractor::mouseReleaseEvent ( QMouseEvent* ) {

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
