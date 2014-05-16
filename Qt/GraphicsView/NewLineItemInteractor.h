#ifndef NEWLINEITEMINTERACTOR_H
#define NEWLINEITEMINTERACTOR_H

#include "EditFrameInteractor.h"

class NewLineItemInteractor : public EditFrameInteractor {
	Q_OBJECT;

public:
	NewLineItemInteractor();

	virtual ~NewLineItemInteractor();

	// @Override
    virtual void mousePressEvent ( QMouseEvent* event );

	// @Override
    virtual void mouseReleaseEvent ( QMouseEvent * event );

signals:
	void editDone();
};

#endif
