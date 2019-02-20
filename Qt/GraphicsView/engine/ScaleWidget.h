/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

#include <QWidget>

class GraphicsSheet;

#ifdef ENGINE_LIBRARY
class Q_DECL_EXPORT ScaleWidget : public QWidget {
#else
class Q_DECL_IMPORT ScaleWidget : public QWidget {
#endif

    Q_OBJECT;

    qreal theScale;
    int offset;

    qreal positionIndicator;

    int smallTicksSize;
    int mediumTicksSize;
    int largeTicksSize;

public:
    enum Direction{Vertical, Horizontal};

    ScaleWidget(QWidget* parent, GraphicsSheet* view, Direction dir);

    void setScale(qreal scale);

    void setOffset(int value);

//    qreal snapToTick(int pos);

    void setPos(qreal pos);

protected:
    void paintEvent ( QPaintEvent * event );

private:
    GraphicsSheet* theView;
    Direction direction;
};
