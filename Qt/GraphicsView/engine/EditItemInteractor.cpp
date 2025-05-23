
//#include <QApplication>
#include <QXmlStreamReader>
#include <QClipboard>
#include <QMouseEvent>
//#include <QGraphicsView>
#include <QGraphicsItem>
#include <QUndoCommand>
#include <QMimeData>
#include <QDebug>
#include <math.h>

//#include "Commands.h"
#include "EditItemInteractor.h"
#include "GraphicsSheet.h"
#include "GraphicsScene.h"

EditItemInteractor::EditItemInteractor() : theItem(0), originalAngle(0), enabledHandles(0xffff), editHandle(0)  {
}


EditItemInteractor::~EditItemInteractor() {
}


void EditItemInteractor::mousePressEvent ( QMouseEvent * event ) {
	if (event->button() != Qt::LeftButton) {
		return;
	}

	QPointF scenePos = theView->mapToScene(event->pos());
	GraphicsScene* theScene = dynamic_cast<GraphicsScene*>(theView->scene());

	theItem = theView->getFocusItem();
	if (theItem) {
        editHandle = theItem->getEditHandle(theView, scenePos, enabledHandles);
        if (editHandle == 0) {
            theItem = 0;    // outside of all handles
        } else if (editHandle == 11) {  // TODO: enum/constant!!!
            event->ignore();
        } else {
            offset = theItem->getHandleOffset(editHandle, scenePos);

            // save current values for undo
#if 0
            originalRect.setRect(theFrame->x(), theFrame->y(),
                                 theFrame->rect().width(), theFrame->rect().height());
            originalAngle = theFrame->rotation();
#endif

            theView->setPositionIndicators(scenePos);
        }
	}

	// no item currently selected / focused
	if (theItem == 0) {

	    // currently, single selection only - independent of whether an
	    // item is clicked or not, clear the current selection
	    theView->scene()->clearSelection();
	    theView->scene()->setFocusItem(0);

	    QGraphicsItem* item = theScene->getItemAt(scenePos);
	    theItem = dynamic_cast<InteractableItem*>(item);
	    if (theItem) {
	        theItem->setItemSelected(true);

	        // prepare for immediate MOVE operation
	        editHandle = 1;     // TODO: use common "super" enum
            offset = theItem->getHandleOffset(editHandle, scenePos);
            theItem->setCursor(theView, editHandle);
	    }
	}
}


void EditItemInteractor::hoverOverEvent ( QMouseEvent * event ) {
	QPointF scenePos = theView->mapToScene(event->pos());

	InteractableItem* frameItem = dynamic_cast<InteractableItem*>(theView->getFocusItem());
	if (frameItem) {
		AbstractEditHandle handle = frameItem->getEditHandle(theView, scenePos, enabledHandles);
		frameItem->setCursor(theView, handle);
	} else {
		theView->setCursor(Qt::ArrowCursor);
	}


#if 0
	QPoint pos = QPoint((int) floor(scenePos.x()),
						(int) floor(scenePos.y()));
	theView->setPositionIndicators(pos);
#endif
}


void EditItemInteractor::mouseMoveEvent ( QMouseEvent * event ) {

	// check if currently a drag is in progress
	if (theItem) {
	    if (editHandle == 11) {  // TODO: enum/constant!!!
	        event->ignore();
	    } else {
            // Map the view mouse position into the corresponding scene coordinates
            QPointF scenePos = theView->mapToScene(event->pos());

            // consider the offset from the real drag point within the handle area
            scenePos += offset;

            // snap the position to the defined snap points
            QPointF snappedPos = theView->snap(scenePos);

            // move the handle to the new position
            theItem->moveHandle(editHandle, snappedPos);

            //QPointF positionIndicator();
            theView->setPositionIndicators(snappedPos);
	    }
	} else {
		hoverOverEvent(event);
	}
}


void EditItemInteractor::mouseReleaseEvent ( QMouseEvent * event ) {
    Q_UNUSED(event);

	if (theItem) {
        if (editHandle == 11) {  // TODO: enum/constant!!!
            event->ignore();
        } else {

#if 0
            KollageGraphicsScene* theScene = dynamic_cast<KollageGraphicsScene*>(theView->scene());

            // check if original position/size is different from new position/size
            // if so, then create an undo object. Otherwise, ignore.
            QRectF newRect(theItem->x(), theItem->y(),
                           theItem->rect().width(), theItem->rect().height());
            int newAngle = theItem->rotation();
            if (newRect != originalRect || newAngle != originalAngle) {
                QUndoCommand* undo = new EditDoneCommand(originalRect, originalAngle, theItem);
                theScene->getUndoStack()->push(undo);
            }
#endif

            emit theItem->propertyChanged();
            theView->setPositionIndicators(QPointF(-1, -1));;
        }

		theItem = 0;
	}
}


void EditItemInteractor::paste() {
#if 0
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    if (mimeData->hasText()) {
        QString clipboardText = mimeData->text();


        KollageGraphicsScene* theScene = dynamic_cast<KollageGraphicsScene*>(theView->scene());

		QXmlStreamReader reader(clipboardText);
		QXmlStreamReader::TokenType type = reader.readNext();
		if (type == QXmlStreamReader::StartDocument) {

			// TODO: We require some severe refactoring here:
			// - Implement a generic item factory to create EditableItems
			//   from an XML stream. Should also allow to register new
			//   item types to avoid the need to have multiple switch() / if-then-else constructs
			// - Need to check if the data which was provided is valid
			// - need to implement a separate data container which only contains
			//   a representation of the XML data stream for an EditableItem

			type = reader.readNext();
			if (type == QXmlStreamReader::StartElement) {
				if (reader.name() == "imageFrame") {
					// Create new item - TODO: Layout??
					CellLayout cell;
					RectItem* item = EditableImageframeItem::readExternal(reader, cell);
					item->setPos(item->x() + 10, item->y() + 10);

// TODO!!!!!
//                        item->setZValue(zOrder++);

					// create undo command to add the item
					QUndoCommand* undo = new NewItemCommand(theScene, item, true);
                    theScene->getUndoStack()->push(undo);
				} else if (reader.name() == "textFrame") {
					// Create new item
					RectItem* item = EditableTextItem::readExternal(reader);
					item->setPos(item->x() + 10, item->y() + 10);

// TODO!!!!!
//                        item->setZValue(zOrder++);

					// create undo command to add the item
					QUndoCommand* undo = new NewItemCommand(theScene, item, true);
                    theScene->getUndoStack()->push(undo);
				}
			}
        }
    }
#endif
}
