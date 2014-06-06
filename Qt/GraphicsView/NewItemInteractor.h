#ifndef NEWITEMINTERACTOR_H
#define NEWITEMINTERACTOR_H

#include "EditFrameInteractor.h"
#include "GraphicsSheet.h"  // FACTORY_FUNCTION

class NewItemInteractor : public EditFrameInteractor {
	Q_OBJECT;

	FACTORY_FUNCTION itemFactory;
	// AbstractEditHandle editHandle;

public:
	NewItemInteractor(FACTORY_FUNCTION factory, AbstractEditHandle editHandle);

	virtual ~NewItemInteractor();

	// @Override
    virtual void mousePressEvent ( QMouseEvent* event );

	// @Override
    virtual void mouseReleaseEvent ( QMouseEvent * event );

signals:
	void editDone();
};

#endif
