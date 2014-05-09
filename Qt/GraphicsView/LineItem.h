#ifndef LINEITEM_H
#define LINEITEM_H

#include <QGraphicsRectItem>
#include <QPointF>
#include <QUndoCommand>
#include "InteractableItem.h"

class GraphicsSheet;

class LineItem : public QGraphicsLineItem, public InteractableItem {
public:
	enum EditHandle {NoHandle,
					 P1Handle, P2Handle,
	                 MoveHandle};

	enum EditHandles {P1HandleMask   = 0x1,
					  P2HandleMask   = 0x2,
					  MoveHandleMask = 0x4,

					  AllHandlesMask = P1HandleMask | P2HandleMask |
					  	  	  	  	   MoveHandleMask,

					  BasicHandlesMask = P1HandleMask |
					                     P2HandleMask |
					                     MoveHandleMask
					 };

    LineItem(const QPointF& pos1, const QPointF& pos2, QGraphicsItem * parent = 0);

    unsigned int /*EditHandle*/ getEditHandle(GraphicsSheet* view, const QPointF& pos, unsigned int /*EditHandles*/ enabledHandles = AllHandlesMask);

    void paintHandles(GraphicsSheet* view, QPainter * painter, unsigned int/*EditHandles */ enabledHandles = AllHandlesMask);

    QSizeF getHandleOffset(unsigned int editHandle, const QPointF& scenePos);

    void moveHandle(unsigned int /*EditHandle*/ editHandle, const QPointF& scenePos);

    void setCursor(GraphicsSheet* theView, unsigned int /*EditHandle*/ handle);

    void paintCoordinates(GraphicsSheet* view, QPainter* painter);

    void paintSelectedBorder(GraphicsSheet* view, QPainter * painter);

    void setItemSelected(bool b) { setSelected(b); }
#if 0
    /**
     * Accepts the visit from an ItemVisitor.
     *
     * @param visitor The visitor which is visiting the item.
     */
//    virtual void accept(const ItemVisitor& visitor) = 0;

//    virtual void writeExternal(QXmlStreamWriter& writer) = 0;

   QRectF boundingRect() const;
#endif
protected:

   // @Override
   virtual QPainterPath shape () const;

   // @Override
   void paint ( QPainter * painter, const QStyleOptionGraphicsItem * style, QWidget *widget);

private:
    void calculateHandles(GraphicsSheet* view);

    QRectF p1Handle;
    QRectF p2Handle;

};

#endif
