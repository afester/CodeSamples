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

class GraphicsItem : public QGraphicsRectItem {
public:
    GraphicsItem ( qreal x, qreal y, qreal width, qreal height, QGraphicsItem * parent = 0 );

protected:
    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);
};

class GraphicsView : public QGraphicsView {
    QColor viewColor;

public:
    GraphicsView(QWidget* parent);

    void setColor(const QColor& color);
    QColor getColor();
};

class Ui_MainWindow;

class MainWindow : public QMainWindow {
        Q_OBJECT

public:

        MainWindow(QWidget* parent);
        ~MainWindow();

        Ui_MainWindow* ui;
        QGraphicsScene* scene;
};
