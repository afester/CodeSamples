#ifndef NEWITEMINTERACTOR_H
#define NEWITEMINTERACTOR_H

#include "EditItemInteractor.h"
#include "GraphicsItemFactory.h"

#ifdef ENGINE_LIBRARY
class Q_DECL_EXPORT NewItemInteractor : public EditItemInteractor {
#else
class Q_DECL_IMPORT NewItemInteractor : public EditItemInteractor {
#endif

	Q_OBJECT;

	FACTORY_FUNCTION itemFactory;
	// AbstractEditHandle editHandle;

public:
    NewItemInteractor(FACTORY_FUNCTION factory); // , AbstractEditHandle editHandle);

	virtual ~NewItemInteractor();

	// @Override
    virtual void mousePressEvent ( QMouseEvent* event );

	// @Override
    virtual void mouseReleaseEvent ( QMouseEvent * event );

signals:
	void editDone();
};

#endif
