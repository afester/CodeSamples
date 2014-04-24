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


class EditableItem : public QGraphicsRectItem {
public:
	enum EditHandle {NoHandle,
					 TopLeftHandle, TopHandle, TopRightHandle,
					 LeftHandle, RotationHandle, RightHandle,
					 BottomLeftHandle, BottomHandle, BottomRightHandle};

	enum EditHandles {TopLeftHandleMask = 0x01,
					  TopHandleMask = 0x02,
					  TopRightHandleMask = 0x04,
					  LeftHandleMask = 0x08,
					  RotationHandleMask = 0x10,
					  RightHandleMask = 0x20,
					  BottomLeftHandleMask = 0x40,
					  BottomHandleMask = 0x80,
					  BottomRightHandleMask = 0x100,

					  AllHandlesMask = TopLeftHandleMask | TopHandleMask | TopRightHandleMask |
					  	  	  	  	   LeftHandleMask | RotationHandleMask | RightHandleMask |
					  	  	  	  	   BottomLeftHandleMask | BottomHandleMask | BottomRightHandleMask,

					  BasicHandlesMask = TopHandleMask | LeftHandleMask | RightHandleMask | BottomHandleMask
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

    virtual void calculateDraggers();

    /**
     * Calculates the handle which is under the given scene position.
     *
     * @param pos The item position to check for a handle.
     * @return The handle at the given coordinates, one of the EditHandle enum values
     */
    EditHandle getEditHandle(const QPointF& pos, EditHandles enabledHandles = AllHandlesMask);

    /**
     * Paints the handles for the edit operations.
     *
     * @param painter The painter to use to paint the handles
     */
    void paintHandles(QPainter * painter, EditHandles enabledHandles = AllHandlesMask);

    void paintCoordinates(QPainter* painter);

    /**
     * Paints the selection border.
     *
     * @param painter The painter to use to paint the selection border.
     */
    void paintSelectedBorder(QPainter * painter);

protected:

    // @Override
    virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

private:

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
