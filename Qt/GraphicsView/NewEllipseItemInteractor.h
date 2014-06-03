#ifndef NEWELLIPSEITEMINTERACTOR_H
#define NEWELLIPSEITEMINTERACTOR_H

#include "EditFrameInteractor.h"

class NewEllipseItemInteractor : public EditFrameInteractor {
	Q_OBJECT;

public:
	NewEllipseItemInteractor();

	virtual ~NewEllipseItemInteractor();

	// @Override
    virtual void mousePressEvent ( QMouseEvent* event );

	// @Override
    virtual void mouseReleaseEvent ( QMouseEvent * event );

signals:
	void editDone();
};

#endif
