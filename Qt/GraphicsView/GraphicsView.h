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



class GraphicsView : public QGraphicsView {
	Q_OBJECT;

    QColor viewColor;

    float xDpi;
    float yDpi;

    float zoomScale;
    QSizeF sceneSize;
    bool landscape;

    void updateSize();

protected:
    void drawBackground(QPainter * painter, const QRectF & rect);

    void paintEvent(QPaintEvent * event);

    void setupViewport ( QWidget * viewport );

    QSize sizeHint() const;

public:
    GraphicsView(QWidget* parent);

    void setColor(const QColor& color);
    QColor getColor();

public slots:
    void setZoom(int idx);

    void setSize(int idx);

    void setDirection(int idx);
};

class Ui_MainWindow;

class MainWindow : public QMainWindow {
        Q_OBJECT;

        QAction *actionX;
        QWidget *centralwidget;
        GraphicsView *view;
        QMenuBar *menubar;
        QStatusBar *statusbar;
        QToolBar *toolBar;

public:

        MainWindow(QWidget* parent);
        ~MainWindow();

        QGraphicsScene* scene;

public slots:
		void printInfo();
};
