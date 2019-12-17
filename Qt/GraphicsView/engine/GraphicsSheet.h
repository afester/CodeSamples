/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */
#ifndef GRAPHICSSHEET_H
#define GRAPHICSSHEET_H

#include <QGraphicsView>

class ScaleWidget;
class ScaleEdgeWidget;
class Interactor;
class NewItemInteractor;
class InteractableItem;
class Snapper;

#ifdef ENGINE_LIBRARY
class Q_DECL_EXPORT GraphicsSheet : public QGraphicsView {
#else
class Q_DECL_IMPORT GraphicsSheet : public QGraphicsView {
#endif

    Q_OBJECT;

    QStringList sizeNames;
    QList<QSizeF> sizeDimensions;

    QStringList scaleNames;
    QList<float> scaleLevels;

    QStringList zoomNames;
    QList<float> zoomLevels;

    ScaleWidget* xScale;
    ScaleWidget* yScale;
    ScaleEdgeWidget* edge;

    float xScaleDPI;
    float yScaleDPI;

    float drawScale;    // e.g. 1:2 => 0.5
    float zoomScale;    // e.g. 50% => 0.5
    QSizeF sceneSize;   // e.g. 148x210
    bool landscape;     // true or false (landscape or portrait)

    Interactor* interactor;
    Snapper* snapper;

    void updateSize();

    QHash<QString, QPointF> points;

public:
    GraphicsSheet(QWidget* parent);

    void addSize(const QString& name, const QSizeF& size);

    void addPoint(const QString& name, const QPointF& point);

    void drawCoordinateSystem(QPainter* painter, const QTransform& t = QTransform(), const QColor& c = Qt::black);

    void drawPoints(QPainter* painter, const QHash<QString, QPointF>& points);

    QStringList getSizeNames() const;

    void addZoom(const QString& name, float level);

    QStringList getZoomNames() const;

    void addScale(const QString& name, float scale);

    QStringList getScaleNames() const;

    void setUnit(const QString& unit);

    void setScale(float scale);

    qreal getScale();

    void setZoom(float zoom);

    void setSize(const QSizeF& dimension);

    void setScaleBackground(const QColor& color);

    void setInteractor(Interactor* interactor);

    Interactor* getInteractor();

    InteractableItem* getFocusItem() const;

    void setSnapper(Snapper* snapper);

    virtual QPointF snap(const QPointF& pos);

    void setPositionIndicators(const QPointF& pos);

    /**
     * @return The size of a handle for an interactable item.
     */
    QSizeF getHandleSize();

    NewItemInteractor* createNewItemInteractor(const QString& itemClass);

    QList<QString> getSupportedItemClasses();

public slots:
    void setScale(int idx);

    void setZoom(int idx);

    void setSize(int idx);

    void setDirection(int idx);

private slots:
    void areaMoved();

    void deleteSelectedItems();

protected:
    // @Override
    virtual void drawBackground(QPainter * painter, const QRectF & rect);

    // @Override
    virtual void drawForeground ( QPainter * painter, const QRectF & rect );

    void resizeEvent ( QResizeEvent * event );

    QSize sizeHint() const;

    void mousePressEvent ( QMouseEvent * event );

    void mouseMoveEvent ( QMouseEvent * event );

    void mouseReleaseEvent ( QMouseEvent * event );

    void wheelEvent(QWheelEvent * event );

    void paste();
};

#endif
