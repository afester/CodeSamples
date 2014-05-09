#ifndef INTERACTABLEITEM_H
#define INTERACTABLEITEM_H

class QPointF;
class GraphicsSheet;

class InteractableItem {
public:

    virtual /*EditHandle*/ unsigned int getEditHandle(GraphicsSheet* view, const QPointF& pos, /*EditHandles*/ unsigned int enabledHandles = 0xffff) = 0;

    virtual void paintHandles(GraphicsSheet* view, QPainter * painter, /*EditHandles*/ unsigned int enabledHandles = 0xffff) = 0; // AllHandlesMask);

    virtual QSizeF getHandleOffset(unsigned int editHandle, const QPointF& scenePos) = 0;

    virtual void moveHandle(/*EditHandle*/ unsigned int editHandle, const QPointF& scenePos) = 0;

    virtual void setCursor(GraphicsSheet* theView, /*EditHandle*/ unsigned int handle) = 0;

    virtual void paintSelectedBorder(GraphicsSheet* view, QPainter * painter) = 0;

    virtual void setItemSelected(bool sel) = 0;
};

#endif
