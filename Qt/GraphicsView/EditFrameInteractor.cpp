
#include <QApplication>
#include <QXmlStreamReader>
#include <QClipboard>
#include <QMouseEvent>
#include <QGraphicsView>
#include <QUndoCommand>
#include <QMimeData>
#include <math.h>

//#include "Commands.h"
//#include "KollageGraphicsScene.h"
//#include "EditableImageframeItem.h"
//#include "EditableTextItem.h"
#include "EditFrameInteractor.h"
#include "Log.h"
#include "GraphicsSheet.h"

EditFrameInteractor::EditFrameInteractor() : theFrame(0), originalAngle(0), enabledHandles(EditableItem::AllHandlesMask)  {

}

EditFrameInteractor::~EditFrameInteractor() {

}


void EditFrameInteractor::paintDecorations(EditableItem* item, QPainter* painter) {
//	item->paintHandles(theView, painter, enabledHandles);
//	item->paintCoordinates(theView, painter);
//	item->paintSelectedBorder(theView, painter);
}


void EditFrameInteractor::mousePressEvent ( QMouseEvent * event ) {
	if (event->button() != Qt::LeftButton) {
		return;
	}

	// Log::log(Log::DEBUG, "EditFrameInteractor") << "mousePressEvent";

	// todo: probably create a QGraphicsSceneMouseEvent and pass this already to the interactor!
	QPointF scenePos = theView->mapToScene(event->pos());
	QGraphicsScene* theScene = theView->scene();
	theFrame = theView->getFocusItem();
	if (theFrame) {
        QPointF itemPos = theFrame->mapFromScene(scenePos);
        editHandle = theFrame->getEditHandle(theView, itemPos, enabledHandles);

        QPoint positionIndicator(-1,-1);
        switch(editHandle) {
            case EditableItem::TopLeftHandle :
                    offset = QSizeF(0, 0); // theFrame->x() - scenePos.x(), theFrame->y() - scenePos.y());
                    positionIndicator = QPoint(theFrame->x(), theFrame->y());
                    break;

            case EditableItem::TopHandle :
                    offset = QSizeF(0, 0); // QSizeF(0, theFrame->y() - scenePos.y());
                    positionIndicator = QPoint(-1, theFrame->y());
                    break;

            case EditableItem::TopRightHandle :
                    offset = QSizeF(0, 0); // QSizeF(theFrame->x() + theFrame->rect().width() - scenePos.x(), theFrame->y() - scenePos.y());
                    positionIndicator = QPoint(theFrame->x() + theFrame->rect().width(), theFrame->y());
                    break;

            case EditableItem::LeftHandle :
                    offset = QSizeF(0, 0); // QSizeF(theFrame->x() - scenePos.x(), 0);
                    positionIndicator = QPoint(theFrame->x(), -1);
                    break;

            case EditableItem::RotationHandle :
                    offset = QSizeF(0, 0);
                    break;

            case EditableItem::RightHandle :
                    offset = QSizeF(0, 0); // QSizeF(theFrame->x() + theFrame->rect().width() - scenePos.x(), 0);
                    positionIndicator = QPoint(theFrame->x() + theFrame->rect().width(), -1);
                    break;

            case EditableItem::BottomLeftHandle :
                    offset = QSizeF(0, 0); // QSizeF(theFrame->x() - scenePos.x(), theFrame->y() + theFrame->rect().height() - scenePos.y());
                    positionIndicator = QPoint(theFrame->x(), theFrame->y() + theFrame->rect().height());
                    break;

            case EditableItem::BottomHandle :
                    offset = QSizeF(0, 0); // QSizeF(0, theFrame->y() + theFrame->rect().height() - scenePos.y());
                    positionIndicator = QPoint(-1, theFrame->y() + theFrame->rect().height());
                    break;

            case EditableItem::BottomRightHandle :
                    offset = QSizeF(0, 0); // QSizeF(theFrame->x() + theFrame->rect().width() - scenePos.x(),
                                   //theFrame->y() + theFrame->rect().height() - scenePos.y());
                    positionIndicator = QPoint(theFrame->x() + theFrame->rect().width(),
                                               theFrame->y() + theFrame->rect().height());
                    break;

            case EditableItem::MoveHandle:
                    offset = QSize(theFrame->x() - scenePos.x(), theFrame->y() - scenePos.y());
                    break;

            default: theFrame = 0;    // outside of all handles
                    break;
        }

if (theFrame) {
        // save current values for undo
        originalRect.setRect(theFrame->x(), theFrame->y(),
                             theFrame->rect().width(), theFrame->rect().height());
        originalAngle = theFrame->rotation();
#if 0
        theView->setPositionIndicators(positionIndicator);
#endif
}

	}

	if (theFrame == 0) {

	    // currently, single selection only - independent of whether a
	    // frame is clicked or not, clear the current selection
	    theView->scene()->clearSelection();

	    QGraphicsItem* item = theScene->itemAt(scenePos, QTransform());
	    theFrame = dynamic_cast<EditableItem*>(item);
	    if (theFrame) {
	        theFrame->setSelected(true);

	        // prepare for immediate MOVE operation
            offset = QSize(theFrame->x() - scenePos.x(), theFrame->y() - scenePos.y());
	    }
	}
}



void EditFrameInteractor::hoverOverEvent ( QMouseEvent * event ) {
	QPointF scenePos = theView->mapToScene(event->pos());

	EditableItem* frameItem = theView->getFocusItem();
	if (frameItem) {
        QPointF itemPos = frameItem->mapFromScene(scenePos);
		EditableItem::EditHandle handle = frameItem->getEditHandle(theView, itemPos, enabledHandles);
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


static qreal dist_Point_to_Segment(const QPointF& P, const QPointF& P0, const QPointF& P1) {

     QVector2D v(P1 - P0);
     QVector2D w(P - P0);

     qreal c1 = QVector2D::dotProduct(w, v);
     if ( c1 <= 0 )
          return QLineF(P, P0).length();


     qreal c2 = QVector2D::dotProduct(v,v);
     if ( c2 <= c1 )
          return QLineF(P, P1).length();

     qreal b = c1 / c2;

     QPointF Pb(P0 + (b * v).toPointF());

     return QLineF(P, Pb).length();
}


void EditFrameInteractor::mouseMoveEvent ( QMouseEvent * event ) {
    QPointF scenePos = theView->mapToScene(event->pos());

	// check if currently a drag is in progress
	if (theFrame) {
        QPointF pos = scenePos + QPointF(offset.width(), offset.height());
        QPoint positionIndicator(-1,-1);
        theFrame->moveHandle(editHandle, pos);

#if 0
        // TODO HACK: special handling of text frames - make sure that the child is always centered
        EditableTextItem* textItem = dynamic_cast<EditableTextItem*>(theFrame);
        if (textItem) {
        	textItem->centerTextItem();
        }



        theView->setPositionIndicators(positionIndicator);

#endif

	} else {
		hoverOverEvent(event);
	}
}


void EditFrameInteractor::mouseReleaseEvent ( QMouseEvent * event ) {
	// Log::log(Log::DEBUG, "EditFrameInteractor") << "mouseReleaseEvent";

	if (theFrame) {
#if 0
	    KollageGraphicsScene* theScene = dynamic_cast<KollageGraphicsScene*>(theView->scene());

        // check if original position/size is different from new position/size
        // if so, then create an undo object. Otherwise, ignore.
        QRectF newRect(theFrame->x(), theFrame->y(),
                       theFrame->rect().width(), theFrame->rect().height());
        int newAngle = theFrame->rotation();
        if (newRect != originalRect || newAngle != originalAngle) {
            QUndoCommand* undo = new EditDoneCommand(originalRect, originalAngle, theFrame);
            theScene->getUndoStack()->push(undo);
        }

    	QPointF scenePos = theView->mapToScene(event->pos());
        QPoint positionIndicator = QPoint((int) floor(scenePos.x()),
                            			  (int) floor(scenePos.y()));
        theView->setPositionIndicators(positionIndicator);
#endif

		theFrame = 0;
	}
}


void EditFrameInteractor::paste() {
#if 0
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    if (mimeData->hasText()) {
        QString clipboardText = mimeData->text();

		Log::log(Log::DEBUG, "KollageGraphicsScene") << "Paste new item into scene: " << clipboardText;

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
					EditableItem* item = EditableImageframeItem::readExternal(reader, cell);
					item->setPos(item->x() + 10, item->y() + 10);

// TODO!!!!!
//                        item->setZValue(zOrder++);

					// create undo command to add the item
					QUndoCommand* undo = new NewItemCommand(theScene, item, true);
                    theScene->getUndoStack()->push(undo);
				} else if (reader.name() == "textFrame") {
					// Create new item
					EditableItem* item = EditableTextItem::readExternal(reader);
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
