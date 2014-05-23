#ifndef INTERACTABLEITEM_H
#define INTERACTABLEITEM_H

class QPointF;
class GraphicsSheet;

typedef unsigned int AbstractEditHandle;


class InteractableItem {
public:

    virtual ~InteractableItem() {};

    virtual AbstractEditHandle getEditHandle(GraphicsSheet* view, const QPointF& pos, AbstractEditHandle enabledHandles = 0xffff) = 0;

    virtual void paintHandles(GraphicsSheet* view, QPainter * painter, AbstractEditHandle enabledHandles = 0xffff) = 0;

    virtual QPointF getHandleOffset(AbstractEditHandle editHandle, const QPointF& scenePos) = 0;

    virtual void moveHandle(AbstractEditHandle editHandle, const QPointF& scenePos) = 0;

    virtual void setCursor(GraphicsSheet* theView, AbstractEditHandle handle) = 0;

    virtual void paintSelectedBorder(GraphicsSheet* view, QPainter * painter) = 0;

    virtual void setItemSelected(bool sel) = 0;
};

#endif
