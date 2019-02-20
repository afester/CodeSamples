#ifndef INTERACTOR_H
#define INTERACTOR_H

#include <QObject>

class QPainter;
class QMouseEvent;
class QWheelEvent;
class RectItem;
class GraphicsSheet;

#ifdef ENGINE_LIBRARY
class Q_DECL_EXPORT Interactor : public QObject {
#else
class Q_DECL_EXPORT Interactor : public QObject {
#endif

protected:
	// the QGraphicsView where the interactor is attached to
	GraphicsSheet* theView;

public:
	virtual ~Interactor();

	void setView(GraphicsSheet* view);

    virtual void mouseMoveEvent ( QMouseEvent* event ) = 0;

    virtual void mousePressEvent ( QMouseEvent* event ) = 0;

    virtual void mouseReleaseEvent ( QMouseEvent* event ) = 0;

    virtual void wheelEvent ( QWheelEvent* event );

    virtual void paste();
};


#endif
