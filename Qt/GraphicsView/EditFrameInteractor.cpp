
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
                    offset = QSizeF(theFrame->x() - scenePos.x(), theFrame->y() - scenePos.y());
                    positionIndicator = QPoint(theFrame->x(), theFrame->y());
                    break;

            case EditableItem::TopHandle :
                    offset = QSizeF(0, theFrame->y() - scenePos.y());
                    positionIndicator = QPoint(-1, theFrame->y());
                    break;

            case EditableItem::TopRightHandle :
                    offset = QSizeF(theFrame->x() + theFrame->rect().width() - scenePos.x(), theFrame->y() - scenePos.y());
                    positionIndicator = QPoint(theFrame->x() + theFrame->rect().width(), theFrame->y());
                    break;

            case EditableItem::LeftHandle :
                    offset = QSizeF(theFrame->x() - scenePos.x(), 0);
                    positionIndicator = QPoint(theFrame->x(), -1);
                    break;

            case EditableItem::RotationHandle :
                    offset = QSizeF(0, 0);
                    break;

            case EditableItem::RightHandle :
                    offset = QSizeF(theFrame->x() + theFrame->rect().width() - scenePos.x(), 0);
                    positionIndicator = QPoint(theFrame->x() + theFrame->rect().width(), -1);
                    break;

            case EditableItem::BottomLeftHandle :
                    offset = QSizeF(theFrame->x() - scenePos.x(), theFrame->y() + theFrame->rect().height() - scenePos.y());
                    positionIndicator = QPoint(theFrame->x(), theFrame->y() + theFrame->rect().height());
                    break;

            case EditableItem::BottomHandle :
                    offset = QSizeF(0, theFrame->y() + theFrame->rect().height() - scenePos.y());
                    positionIndicator = QPoint(-1, theFrame->y() + theFrame->rect().height());
                    break;

            case EditableItem::BottomRightHandle :
                    offset = QSizeF(theFrame->x() + theFrame->rect().width() - scenePos.x(),
                                   theFrame->y() + theFrame->rect().height() - scenePos.y());
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
		switch(handle) {
			case EditableItem::TopLeftHandle :
					theView->setCursor(Qt::SizeFDiagCursor);
					break;

			case EditableItem::TopHandle :
					theView->setCursor(Qt::SizeVerCursor);
					break;

			case EditableItem::TopRightHandle :
					theView->setCursor(Qt::SizeBDiagCursor);
					break;

			case EditableItem::LeftHandle :
					theView->setCursor(Qt::SizeHorCursor);
					break;

			case EditableItem::RotationHandle :
					theView->setCursor(Qt::PointingHandCursor);  // TODO: Create rotation cursor
					break;

			case EditableItem::RightHandle :
					theView->setCursor(Qt::SizeHorCursor);
					break;

			case EditableItem::BottomLeftHandle :
					theView->setCursor(Qt::SizeBDiagCursor);
					break;

			case EditableItem::BottomHandle :
					theView->setCursor(Qt::SizeVerCursor);
					break;

			case EditableItem::BottomRightHandle :
					theView->setCursor(Qt::SizeFDiagCursor);
					break;

            case EditableItem::MoveHandle :
                    theView->setCursor(Qt::OpenHandCursor);
                    break;

            default :
                    theView->setCursor(Qt::ArrowCursor);
					break;
		}
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
	    QPointF itemPos = theFrame->mapFromScene(scenePos);

        QPointF pos = scenePos + QPointF(offset.width(), offset.height());

        // map mouse pointer scene position to item coordinates
        QPointF posInItem = theFrame->mapFromScene(scenePos);

        QPointF newPos;
        QSizeF newSize;

        QPoint positionIndicator(-1,-1);

        switch(editHandle) {
			case EditableItem::TopLeftHandle :{
			        // P1 is the new upper left corner (in scene coordinates)
			        QPointF P1 = scenePos;

			        // P2 is the fix point (in scene coordinates)
                    QPointF P2 = theFrame->mapToScene(theFrame->rect().width(),
                                                      theFrame->rect().height());

                    // transform coordinate system so that its center is at P1
                    // we need to transform back (from global coordinates into local coordinates),
                    // so we need to do the inverse operations (rotate -angle => translate -P1)
                    QTransform t;
                    t.rotate(-theFrame->rotation());
                    t.translate(-P1.x(), -P1.y());

                    // Map the fix point into the transformed coordinate system -
                    // this will be the new width and height
                    QPointF P2i = t.map(P2);
                    newSize = QSizeF(P2i.x(), P2i.y());

                    if (newSize.width() < 10) {
                        newSize.setWidth(10);
                    }
                    if (newSize.height() < 10) {
                        newSize.setHeight(10);
                    }

                    // calculate new center point - this will become the new transformation center point
                    QPointF P3(newSize.width()/2, newSize.height()/2);
                    theFrame->setTransformOriginPoint(P3);

                    // transform P1 to the new item position
                    QTransform t2;
                    t2.translate(P1.x(), P1.y());
                    t2.rotate(theFrame->rotation());
                    t2.translate(P3.x(), P3.y());
                    t2.rotate(-theFrame->rotation());
                    t2.translate(-P3.x(), -P3.y());
                    t2.translate(-P1.x(), -P1.y());
                    newPos = t2.map(P1);

					positionIndicator = QPoint(newPos.x(), newPos.y());
					break;
			}
/*****************************/
			case EditableItem::TopHandle :

					//newSize = QSizeF(theFrame->rect().width(), theFrame->rect().height() + theFrame->y() - pos.y());
			        newSize = QSizeF(theFrame->rect().width(), theFrame->rect().height() - posInItem.y());
			        if (newSize.height() < 10) {
						newSize.setHeight(10);
					}
					newPos = QPointF(theFrame->x(), theFrame->y() + posInItem.y()); // theFrame->y() + theFrame->rect().height() - newSize.height());

/**********************************/
					positionIndicator = QPoint(-1, newPos.y());
					break;

			case EditableItem::TopRightHandle :
					newSize = QSizeF(pos.x() - theFrame->x(), theFrame->rect().height() + theFrame->y() - pos.y());
					if (newSize.width() < 10) {
						newSize.setWidth(10);
					}
					if (newSize.height() < 10) {
						newSize.setHeight(10);
					}
					newPos = QPointF(theFrame->x(), theFrame->y() + theFrame->rect().height() - newSize.height());

					positionIndicator = QPoint(newPos.x() + newSize.width(), newPos.y());
					break;

			case EditableItem::LeftHandle :
                	newSize = QSizeF(theFrame->rect().width() + theFrame->x() - pos.x(), theFrame->rect().height());
                	if (newSize.width() < 10) {
                		newSize.setWidth(10);
                	}
                	newPos = QPointF(theFrame->x() + theFrame->rect().width() - newSize.width() , theFrame->y());

                	positionIndicator = QPoint(newPos.x(), -1);
                	break;

			case EditableItem::RotationHandle : {
						// position and size remains the same
						newPos = theFrame->pos();
						newSize = QSizeF(theFrame->rect().width(), theFrame->rect().height());

						// calculate center of rectangle (scene coordinates)
						QPointF center = QPointF(theFrame->pos().x() + theFrame->rect().width() / 2,
												 theFrame->pos().y() + theFrame->rect().height() / 2);

						qreal deltaX = pos.x() - center.x();
						qreal deltaY = center.y() - pos.y();

						// TODO: RECHECK ANGLE CALCULATION!!!!!! Does not correctly work yet!

						int angle = (atan(deltaX / deltaY) * 360) / (2*M_PI);

						if (deltaY < 0 && deltaX >= 0) {
							angle = 180 - abs(angle);
						} else if (deltaY < 0 && deltaX <= 0) {
							angle = 180 + angle;
						} else if (deltaY >= 0 && deltaX <= 0) {
							angle = 360 + angle;
						}

						// qDebug() << rect() << "/" << center << ", " << deltaX << "/" << deltaY << "=> " << angle;

						// calculate center of rectangle (item coordinates)
						QPointF center2 = QPointF(theFrame->rect().width() / 2, theFrame->rect().height() / 2);
						theFrame->setTransformOriginPoint(center2);
						theFrame->setRotation(angle);
					}
					break;

			case EditableItem::RightHandle :
					// newSize = QSizeF(pos.x() - theFrame->x(), theFrame->rect().height());
		            newSize = QSizeF(posInItem.x(), theFrame->rect().height());
					if (newSize.width() < 10) {
						newSize.setWidth(10);
					}
					// newPos = QPointF(theFrame->x(), theFrame->y());
		            newPos = QPointF(theFrame->x(), theFrame->y());

					positionIndicator = QPoint(newPos.x() + newSize.width(), -1);
					break;

			case EditableItem::BottomLeftHandle :
					newSize = QSizeF(theFrame->rect().width() - posInItem.x(),
					                 posInItem.y()); //  - theFrame->y());
					if (newSize.width() < 10) {
						newSize.setWidth(10);
					}
					if (newSize.height() < 10) {
						newSize.setHeight(10);
					}
					//newPos = QPointF(theFrame->x() + theFrame->rect().width() - newSize.width() , theFrame->y());
					newPos = QPointF(theFrame->x() + posInItem.x(), theFrame->y());

					positionIndicator = QPoint(newPos.x(), newPos.y() + newSize.height());
					break;

			case EditableItem::BottomHandle :
                    newSize = QSizeF(theFrame->rect().width(), posInItem.y());
                    if (newSize.height() < 10) {
                        newSize.setHeight(10);
                    }
                    newPos = QPointF(theFrame->x(), theFrame->y());

                    positionIndicator = QPoint(-1, newPos.y() + newSize.height());
					break;

			case EditableItem::BottomRightHandle :
			        newSize = QSizeF(posInItem.x(), posInItem.y());
					if (newSize.width() < 10) {
						newSize.setWidth(10);
					}
					if (newSize.height() < 10) {
						newSize.setHeight(10);
					}
					newPos = QPointF(theFrame->x(), theFrame->y());

					positionIndicator = QPoint(newPos.x() + newSize.width(), newPos.y() + newSize.height());
					break;

			default :   /* Move */
					newPos = QPointF(pos.x(), pos.y());
					newSize = QSizeF(theFrame->rect().width(), theFrame->rect().height());

					positionIndicator = QPoint(newPos.x(), newPos.y());
					break;
        }

        QRectF newRect(QPointF(0,0), newSize);
        //qDebug() << "NEW RECT:" << newRect;
        theFrame->setRect(newRect);
        theFrame->setPos(newPos);

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
