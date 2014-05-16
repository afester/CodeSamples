#ifndef NEWTEXTITEMINTERACTOR_H
#define NEWTEXTITEMINTERACTOR_H

#include "EditFrameInteractor.h"

class NewTextItemInteractor : public EditFrameInteractor {
	Q_OBJECT;

public:
	NewTextItemInteractor();

	virtual ~NewTextItemInteractor();

	// @Override
    virtual void mousePressEvent ( QMouseEvent* event );

	// @Override
    virtual void mouseReleaseEvent ( QMouseEvent * event );

signals:
	void editDone();
};

#endif
