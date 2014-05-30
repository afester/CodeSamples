#ifndef LINEITEM_H
#define LINEITEM_H

#include <QGraphicsRectItem>
#include <QPointF>
#include <QUndoCommand>
#include "InteractableItem.h"

class GraphicsSheet;

class LineItem : public QGraphicsLineItem, public InteractableItem {
public:
	enum EditHandle {NoHandle, MoveHandle,  // TODO: move in common "super" enum
					 P1Handle, P2Handle};

	enum EditHandles {MoveHandleMask = 0x1,
	                  P1HandleMask   = 0x2,
					  P2HandleMask   = 0x4,

					  AllHandlesMask = P1HandleMask | P2HandleMask |
					  	  	  	  	   MoveHandleMask,

					  BasicHandlesMask = P1HandleMask |
					                     P2HandleMask |
					                     MoveHandleMask
					 };

    LineItem(const QPointF& pos1, const QPointF& pos2, QGraphicsItem * parent = 0);

    AbstractEditHandle getEditHandle(GraphicsSheet* view, const QPointF& pos, AbstractEditHandle enabledHandles = AllHandlesMask);

    void paintHandles(GraphicsSheet* view, QPainter * painter, AbstractEditHandle enabledHandles = AllHandlesMask);

    QPointF getHandleOffset(AbstractEditHandle editHandle, const QPointF& scenePos);

    void moveHandle(AbstractEditHandle editHandle, const QPointF& scenePos);

    void setCursor(GraphicsSheet* theView, AbstractEditHandle handle);

    void paintCoordinates(GraphicsSheet* view, QPainter* painter);

    void paintSelectedBorder(GraphicsSheet* view, QPainter * painter);

    void setItemSelected(bool b) { setSelected(b); }

    // @Override
    virtual QPointF getNearestEdge(GraphicsSheet* theView, const QPointF& scenePos);

#if 0
    /**
     * Accepts the visit from an ItemVisitor.
     *
     * @param visitor The visitor which is visiting the item.
     */
//    virtual void accept(const ItemVisitor& visitor) = 0;

//    virtual void writeExternal(QXmlStreamWriter& writer) = 0;

#endif
protected:

   // @Override
   virtual QRectF boundingRect() const;

   // @Override
   virtual QPainterPath shape () const;

   // @Override
   void paint ( QPainter * painter, const QStyleOptionGraphicsItem * style, QWidget *widget);
};

#endif
