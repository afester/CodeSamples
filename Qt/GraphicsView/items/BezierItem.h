#ifndef BEZIERITEM_H
#define BEZIERITEM_H

#include <QGraphicsRectItem>
#include <QPointF>
#include <QUndoCommand>
#include "InteractableItem.h"

class GraphicsSheet;

class BezierItem : public QGraphicsPathItem, public InteractableItem {
public:
	enum EditHandle {NoHandle, MoveHandle,  // TODO: move in common "super" enum
					 P1Handle, P2Handle, C1Handle, C2Handle};

	enum EditHandles {MoveHandleMask = 0x1,
	                  P1HandleMask   = 0x2,
					  P2HandleMask   = 0x4,
                      C1HandleMask   = 0x8,
                      C2HandleMask   = 0x10,

					  AllHandlesMask = P1HandleMask | P2HandleMask |
					  	  	  	  	   C1HandleMask | C2HandleMask | MoveHandleMask,

					  BasicHandlesMask = P1HandleMask |
					                     P2HandleMask |
                                         C1HandleMask |
                                         C2HandleMask |
					                     MoveHandleMask
					 };

	BezierItem();

  //  BezierItem(const QPointF& pos1, const QPointF& pos2, QGraphicsItem * parent = 0);

    /**
     * Factory function to create a BezierItem instance.
     */
    static QGraphicsItem* create();


    AbstractEditHandle getEditHandle(GraphicsSheet* view, const QPointF& pos, AbstractEditHandle enabledHandles = AllHandlesMask);

    virtual AbstractEditHandle getNewHandle();

    void paintHandles(GraphicsSheet* view, QPainter * painter, AbstractEditHandle enabledHandles = AllHandlesMask);

    QPointF getHandleOffset(AbstractEditHandle editHandle, const QPointF& scenePos);

    void moveHandle(AbstractEditHandle editHandle, const QPointF& scenePos);

    void setCursor(GraphicsSheet* theView, AbstractEditHandle handle);

    void paintCoordinates(GraphicsSheet* view, QPainter* painter);

    void paintSelectedBorder(GraphicsSheet* view, QPainter * painter);

    void setItemSelected(bool b) { setSelected(b); }

    // @Override
    virtual QPointF getNearestEdge(GraphicsSheet* theView, const QPointF& scenePos);

    /**
     * Accepts the visit from an ItemVisitor.
     *
     * @param visitor The visitor which is visiting the item.
     */
//    virtual void accept(const ItemVisitor& visitor) = 0;

    // @Override
    virtual void writeExternal(QXmlStreamWriter& writer);

    // @Override
    virtual void readExternal(QXmlStreamReader& reader);

protected:

   // @Override
   virtual QRectF boundingRect() const;

   // @Override
   virtual QPainterPath shape () const;

   // @Override
   void paint ( QPainter * painter, const QStyleOptionGraphicsItem * style, QWidget *widget);

private:
   QPainterPath path;
   QPointF p2;
   QPointF c1;
   QPointF c2;
};

#endif
