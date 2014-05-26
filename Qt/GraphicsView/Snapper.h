#ifndef SNAPPER_H
#define SNAPPER_H

#include <QPointF>

class GraphicsSheet;

class Snapper {
    GraphicsSheet* theView;

public:
	virtual ~Snapper();

	void setView(GraphicsSheet* view);

	virtual QPointF snap(const QPointF& pos) = 0;
};


class UnitSnapper : public Snapper {
public:
    virtual QPointF snap(const QPointF& pos);
};


class GridSnapper : public Snapper {

};


class EdgeSnapper : public Snapper {

};


class EdgeAndGridSnapper : public Snapper {

};


#endif
