#ifndef EDITFRAMEINTERACTOR_H
#define EDITFRAMEINTERACTOR_H


#include <QSize>
#include <QRectF>

#include "Interactor.h"
#include "EditableItem.h"


class EditFrameInteractor : public Interactor {
protected:
	EditableItem::EditHandles enabledHandles;

	EditableItem* theFrame;
	EditableItem::EditHandle editHandle;
    QSize offset;       // mouse offset when resizing the item
    QRectF originalRect;  // original rect for undo operation
    int originalAngle;    // original rotation angle for undo operation

public:
	EditFrameInteractor();

	virtual ~EditFrameInteractor();

	void hoverOverEvent ( QMouseEvent * event );

	// @Override
	virtual void paintDecorations(EditableItem* item, QPainter* painter);

	// @Override
    virtual void mouseMoveEvent ( QMouseEvent* event );

	// @Override
    virtual void mousePressEvent ( QMouseEvent* event );

	// @Override
    virtual void mouseReleaseEvent ( QMouseEvent* event );

    // @Override
    virtual void paste();
};


#endif
