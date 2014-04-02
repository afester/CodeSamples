/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

/* StackOverflow reference:
 * http://stackoverflow.com/questions/...
 */

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>

class GraphicsItem : public QGraphicsRectItem {
public:
    GraphicsItem ( qreal x, qreal y, qreal width, qreal height, QGraphicsItem * parent = 0 );

protected:
    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
};

class GraphicsScene : public QGraphicsScene {
};

class GraphicsView : public QGraphicsView {
	Q_OBJECT;

    QColor viewColor;


    float xDpi;
    float yDpi;

    float zoomScale;    // e.g. 50%
    QSizeF sceneSize;   // e.g. 148x210
    bool landscape;     // true or false (landscape or portrait)

    void updateSize();

protected:
    void drawBackground(QPainter * painter, const QRectF & rect);

    void resizeEvent ( QResizeEvent * event );


    QSize sizeHint() const;

public:
    GraphicsView(QWidget* parent);

    void setZoom(float zoom);

    void setSize(const QSizeF& dimension);

    void setDirection(int idx);

    void setColor(const QColor& color);

    QColor getColor();
};


class ScaleWidget : public QWidget {
    Q_OBJECT;

    float theScale;
    int offset;

    int smallTicksSize;
    int mediumTicksSize;
    int largeTicksSize;

public:
    enum Direction{Vertical, Horizontal};

    ScaleWidget(QWidget* parent, GraphicsView* view, Direction dir);

    void setScale(float scale);

    void setOffset(int value);

protected:
    void paintEvent ( QPaintEvent * event );

private:
    GraphicsView* theView;
    Direction direction;
};


class ScaleEdgeWidget;

class GraphicsSheet : public QFrame {
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
    GraphicsView *view;

public:
    GraphicsScene* scene;

public:
    GraphicsSheet(QWidget* parent);

    void addSize(const QString& name, const QSizeF& size);

    QStringList getSizeNames() const;

    void addZoom(const QString& name, float level);

    QStringList getZoomNames() const;

    void addScale(const QString& name, float scale);

    QStringList getScaleNames() const;

    void setUnit(const QString& unit);

    GraphicsView* getView();

    GraphicsScene* getScene();

    void updateScales();

public slots:
    void setScale(int idx);

    void setZoom(int idx);

    void setSize(int idx);

    void setDirection(int idx);

private slots:
    void areaMoved();
};


class Ui_MainWindow;

class MainWindow : public QMainWindow {
    Q_OBJECT;

    QAction *actionX;
    QWidget *centralwidget;
    GraphicsSheet* graphicsSheet;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QToolBar *toolBar;

public:

    MainWindow(QWidget* parent);
    ~MainWindow();


public slots:
	void printInfo();
};
