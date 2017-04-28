/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

#include <QWidget>
#include <QPixmap>

class QPainter;
class QPaintEvent;

class Canvas : public QWidget {
	QPainter* thePainter;
	QPixmap pixmap;

protected:
	void paintEvent(QPaintEvent * event);

public:
	Canvas(QWidget* parent, int width, int height);

    void drawLine(int x1, int y1, int x2, int y2);
    void drawEllipse(int x, int y, int w, int h);
    void drawRectangle(int x, int y, int w, int h);
};
