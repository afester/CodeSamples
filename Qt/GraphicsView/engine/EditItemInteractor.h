#ifndef EDITITEMINTERACTOR_H
#define EDITITEMINTERACTOR_H


#include <QSize>
#include <QRectF>

#include "Interactor.h"
#include "InteractableItem.h"

#ifdef ENGINE_LIBRARY
class Q_DECL_EXPORT EditItemInteractor : public Interactor {
#else
class Q_DECL_IMPORT EditItemInteractor : public Interactor {
#endif

protected:

	InteractableItem* theItem;
    int originalAngle;    // original rotation angle for undo operation
    AbstractEditHandle enabledHandles;

	AbstractEditHandle editHandle;
    QPointF offset;       // mouse offset when resizing the item
    QRectF originalRect;  // original rect for undo operation

public:
	EditItemInteractor();

	virtual ~EditItemInteractor();

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
