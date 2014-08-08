#ifndef EDITABLEITEM_H
#define EDITABLEITEM_H

#include <QGraphicsRectItem>
#include <QPointF>
#include <QUndoCommand>
#include <QBrush>
#include <QPen>

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

// http://qt-project.org/doc/qt-4.8/moc.html
// Multiple Inheritance Requires QObject to Be First
// If you are using multiple inheritance, moc assumes
// that the first inherited class is a subclass of QObject.
// Also, be sure that only the first inherited class is a QObject.
class RectItem : public InteractableItem, public QGraphicsRectItem {
    Q_OBJECT;

    Q_PROPERTY(QPointF pos   READ pos   WRITE setPos);
    Q_PROPERTY(QPen    pen   READ pen   WRITE setPen);
    Q_PROPERTY(QBrush  brush READ brush WRITE setBrush);

public:
	enum EditHandle {NoHandle, MoveHandle,              // TODO: Move in common "super" enum
					 TopLeftHandle, TopHandle, TopRightHandle,
					 LeftHandle, RotationHandle, RightHandle,
					 BottomLeftHandle, BottomHandle, BottomRightHandle};

	enum EditHandles {MoveHandleMask        = 0x001,
	                  TopLeftHandleMask     = 0x002,
					  TopHandleMask         = 0x004,
					  TopRightHandleMask    = 0x008,
					  LeftHandleMask        = 0x010,
					  RotationHandleMask    = 0x020,
					  RightHandleMask       = 0x040,
					  BottomLeftHandleMask  = 0x080,
					  BottomHandleMask      = 0x100,
					  BottomRightHandleMask = 0x200,

					  AllHandlesMask = TopLeftHandleMask | TopHandleMask | TopRightHandleMask |
					  	  	  	  	   LeftHandleMask | RotationHandleMask | RightHandleMask |
					  	  	  	  	   BottomLeftHandleMask | BottomHandleMask | BottomRightHandleMask|
					  	  	  	  	   MoveHandleMask,

					  BasicHandlesMask = TopHandleMask | LeftHandleMask |
					                     RightHandleMask | BottomHandleMask |
					                     MoveHandleMask
					 };


	/**
	 * Default constructor for deserialization
	 */
	RectItem();

    RectItem(const QPointF& pos, QGraphicsItem * parent = 0);

    RectItem(const QRectF& rect, QGraphicsItem * parent = 0);

    /**
     * Factory function to create a RectItem instance.
     */
    static QGraphicsItem* create();

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

    virtual void calculateHandles(GraphicsSheet* view);

    // @Override
    virtual QRectF boundingRect() const;

    // @Override
    virtual QPainterPath shape () const;

private:
    void activateGeometry(const QSizeF& newSize, const QPointF& pos1);

    void moveTopLeftHandle(const QPointF& scenePos);
    void moveRotationHandle(const QPointF& scenePos);
    void moveRightHandle(const QPointF& scenePos);
    void moveBottomHandle(const QPointF& scenePos);
    void moveBottomRightHandle(const QPointF& scenePos);
    void moveTopHandle(const QPointF& scenePos);
    void moveTopRightHandle(const QPointF& scenePos);
    void moveLeftHandle(const QPointF& scenePos);
    void moveBottomLeftHandle(const QPointF& scenePos);

    // The item coordinates of each edit handle
    QPointF rotationHandle;
    QPointF topLeft;
    QPointF topRight;
    QPointF bottomLeft;
    QPointF bottomRight;
    QPointF top;
    QPointF bottom;
    QPointF left;
    QPointF right;
};

#endif
