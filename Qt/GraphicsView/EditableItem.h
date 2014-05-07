#ifndef EDITABLEITEM_H
#define EDITABLEITEM_H

#include <QGraphicsRectItem>
#include <QPointF>
#include <QUndoCommand>

#if 0
class QXmlStreamWriter;


class EditableTextItem;
class EditableImageframeItem;

class ItemVisitor {
public:
    virtual void visitSelectedItems(QGraphicsScene* scene);

    virtual void visit(EditableTextItem* item) const;

    virtual void visit(EditableImageframeItem* item) const;
};
#endif


class GraphicsSheet;

class EditableItem : public QGraphicsRectItem {
public:
	enum EditHandle {NoHandle,
					 TopLeftHandle, TopHandle, TopRightHandle,
					 LeftHandle, RotationHandle, RightHandle,
					 BottomLeftHandle, BottomHandle, BottomRightHandle,
	                 MoveHandle};

	enum EditHandles {TopLeftHandleMask = 0x01,
					  TopHandleMask = 0x02,
					  TopRightHandleMask = 0x04,
					  LeftHandleMask = 0x08,
					  RotationHandleMask = 0x10,
					  RightHandleMask = 0x20,
					  BottomLeftHandleMask = 0x40,
					  BottomHandleMask = 0x80,
					  BottomRightHandleMask = 0x100,
					  MoveHandleMask = 0x200,

					  AllHandlesMask = TopLeftHandleMask | TopHandleMask | TopRightHandleMask |
					  	  	  	  	   LeftHandleMask | RotationHandleMask | RightHandleMask |
					  	  	  	  	   BottomLeftHandleMask | BottomHandleMask | BottomRightHandleMask|
					  	  	  	  	   MoveHandleMask,

					  BasicHandlesMask = TopHandleMask | LeftHandleMask |
					                     RightHandleMask | BottomHandleMask |
					                     MoveHandleMask
					 };

    EditableItem(const QPointF& pos, QGraphicsItem * parent = 0);

    EditableItem(const QRectF & rect, QGraphicsItem * parent = 0);

    /**
     * Accepts the visit from an ItemVisitor.
     *
     * @param visitor The visitor which is visiting the item.
     */
//    virtual void accept(const ItemVisitor& visitor) = 0;

//    virtual void writeExternal(QXmlStreamWriter& writer) = 0;


    /**
     * Calculates the handle which is under the given scene position.
     *
     * @param pos The item position to check for a handle.
     * @return The handle at the given coordinates, one of the EditHandle enum values
     */
    EditHandle getEditHandle(GraphicsSheet* view, const QPointF& pos, EditHandles enabledHandles = AllHandlesMask);

    /**
     * Paints the handles for the edit operations.
     *
     * @param painter The painter to use to paint the handles
     */
    void paintHandles(GraphicsSheet* view, QPainter * painter, EditHandles enabledHandles = AllHandlesMask);

    void moveHandle(EditHandle editHandle, const QPointF& scenePos);

    void setCursor(GraphicsSheet* theView, EditHandle handle);

    void paintCoordinates(GraphicsSheet* view, QPainter* painter);

    /**
     * Paints the selection border.
     *
     * @param painter The painter to use to paint the selection border.
     */
   void paintSelectedBorder(GraphicsSheet* view, QPainter * painter);

   QRectF boundingRect() const;

protected:

    // @Override
    virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

    virtual void calculateDraggers(GraphicsSheet* view);

private:
    void moveTopLeftHandle(const QPointF& scenePos);
    void moveRotationHandle(const QPointF& scenePos);
    void moveRightHandle(const QPointF& scenePos);
    void moveBottomHandle(const QPointF& scenePos);
    void moveBottomRightHandle(const QPointF& scenePos);
    void moveTopHandle(const QPointF& scenePos);
    void moveTopRightHandle(const QPointF& scenePos);
    void moveLeftHandle(const QPointF& scenePos);
    void moveBottomLeftHandle(const QPointF& scenePos);

    QRectF rotationHandle;
    QRectF topLeft;
    QRectF topRight;
    QRectF bottomLeft;
    QRectF bottomRight;
    QRectF top;
    QRectF bottom;
    QRectF left;
    QRectF right;
};

#endif
