#ifndef NEWRECTITEMINTERACTOR_H
#define NEWRECTITEMINTERACTOR_H

#include "EditFrameInteractor.h"

class NewRectItemInteractor : public EditFrameInteractor {
	Q_OBJECT;

public:
	NewRectItemInteractor();

	virtual ~NewRectItemInteractor();

	// @Override
    virtual void mousePressEvent ( QMouseEvent* event );

	// @Override
    virtual void mouseReleaseEvent ( QMouseEvent * event );

signals:
	void editDone();
};

#endif
