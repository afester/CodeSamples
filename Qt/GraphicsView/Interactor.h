#ifndef INTERACTOR_H
#define INTERACTOR_H

#include <QObject>

class QPainter;
class QMouseEvent;
class QWheelEvent;
class EditableItem;
class GraphicsSheet;

class Interactor : public QObject {
protected:
	// the QGraphicsView where the interactor is attached to
	GraphicsSheet* theView;

public:
	virtual ~Interactor();

	void setView(GraphicsSheet* view);

	/**
	 * Paints the item decorations for this interactor.
	 * TODO: not sure if this is a good approach from an architectural perspective
	 */
	virtual void paintDecorations(EditableItem* item, QPainter* painter);

    virtual void mouseMoveEvent ( QMouseEvent* event ) = 0;

    virtual void mousePressEvent ( QMouseEvent* event ) = 0;

    virtual void mouseReleaseEvent ( QMouseEvent* event ) = 0;

    virtual void wheelEvent ( QWheelEvent* event );

    virtual void paste();
};


#endif
