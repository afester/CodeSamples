#ifndef CIRCLEITEM_H
#define CIRCLEITEM_H

#include <QGraphicsRectItem>
#include <QPointF>
#include <QUndoCommand>
#include "InteractableItem.h"

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

class CircleItem : public QGraphicsEllipseItem, public InteractableItem {
public:
	enum EditHandle {NoHandle, MoveHandle,              // TODO: Move in common "super" enum
					 RadHandle};//, LeftHandle, RotationHandle, RightHandle,
					 //BottomHandle};

	enum EditHandles {MoveHandleMask        = 0x001,
					  RadHandleMask         = 0x004,
					  //LeftHandleMask        = 0x010,
					  //RotationHandleMask    = 0x020,
					  //RightHandleMask       = 0x040,
					  //BottomHandleMask      = 0x100,

					  AllHandlesMask = RadHandleMask |
					  	  	  	  	   //LeftHandleMask | RotationHandleMask | RightHandleMask |
					  	  	  	  	   //BottomHandleMask |
					  	  	  	  	   MoveHandleMask,

					  BasicHandlesMask = RadHandleMask |// LeftHandleMask |
					                     //RightHandleMask | BottomHandleMask |
					                     MoveHandleMask
					 };

	CircleItem();

    CircleItem(const QPointF& pos, QGraphicsItem * parent = 0);

    CircleItem(const QPointF& pos, qreal radius, QGraphicsItem * parent = 0);

    /**
     * Factory function to create a CircleItem instance.
     */
    static QGraphicsItem* create();

    /**
     * Accepts the visit from an ItemVisitor.
     *
     * @param visitor The visitor which is visiting the item.
     */
//    virtual void accept(const ItemVisitor& visitor) = 0;

    // @Override
    virtual void readExternal(QXmlStreamReader& writer);

    // @Override
    virtual void writeExternal(QXmlStreamWriter& writer);

    // @Override
    virtual AbstractEditHandle getEditHandle(GraphicsSheet* view, const QPointF& pos, AbstractEditHandle enabledHandles = AllHandlesMask);

    // @Override
    virtual void paintHandles(GraphicsSheet* view, QPainter * painter, AbstractEditHandle enabledHandles = AllHandlesMask);

    // @Override
    virtual QPointF getHandleOffset(AbstractEditHandle editHandle, const QPointF& scenePos);

    // @Override
    virtual void moveHandle(AbstractEditHandle editHandle, const QPointF& scenePos);

    // @Override
    virtual void setCursor(GraphicsSheet* theView, AbstractEditHandle handle);

    // @Override
    virtual QPointF getNearestEdge(GraphicsSheet* theView, const QPointF& scenePos);

    /**
     * Paints the selection border.
     *
     * @param painter The painter to use to paint the selection border.
     */
   void paintSelectedBorder(GraphicsSheet* view, QPainter * painter);

   void setItemSelected(bool b) { setSelected(b); }

protected:

    // @Override
    virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

    // @Override
    virtual QRectF boundingRect() const;

    // @Override
    virtual QPainterPath shape () const;

private:
    void moveRadHandle(const QPointF& scenePos);

    // The item coordinates of each edit handle
    QPointF radHandle;
};

#endif
