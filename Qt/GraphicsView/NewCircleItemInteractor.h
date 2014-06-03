#ifndef NEWCIRCLEITEMINTERACTOR_H
#define NEWCIRCLEITEMINTERACTOR_H

#include "EditFrameInteractor.h"

class NewCircleItemInteractor : public EditFrameInteractor {
	Q_OBJECT;

public:
	NewCircleItemInteractor();

	virtual ~NewCircleItemInteractor();

	// @Override
    virtual void mousePressEvent ( QMouseEvent* event );

	// @Override
    virtual void mouseReleaseEvent ( QMouseEvent * event );

signals:
	void editDone();
};

#endif
