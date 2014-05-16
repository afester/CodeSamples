#ifndef EDITFRAMEINTERACTOR_H
#define EDITFRAMEINTERACTOR_H


#include <QSize>
#include <QRectF>

#include "Interactor.h"
#include "InteractableItem.h"


class EditFrameInteractor : public Interactor {
protected:

	InteractableItem* theItem;
    int originalAngle;    // original rotation angle for undo operation
    AbstractEditHandle enabledHandles;

	AbstractEditHandle editHandle;
    QSizeF offset;       // mouse offset when resizing the item
    QRectF originalRect;  // original rect for undo operation

public:
	EditFrameInteractor();

	virtual ~EditFrameInteractor();

	void hoverOverEvent ( QMouseEvent * event );

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
