/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

#include <QDebug>
#include <QApplication>
#include <QGraphicsItem>
#include <QGridLayout>
#include <QDesktopWidget>

#include "GraphicsView.h"

GraphicsView::GraphicsView(QWidget* parent) : QGraphicsView(parent) {
	setRenderHint(QPainter::Antialiasing);
}


QSize GraphicsView::sizeHint() const {
	QSizeF baseSize = matrix().mapRect(sceneRect()).size();

	qDebug() << "baseSize: " << baseSize;
	// Here, the baseSize can be something like  QSizeF(793.701, 559.37)
	// This should result in sizeHint of 794 x 560 (!!) to make sure the whole scene can be shown
	// However, toSize() below rounds to nearest integer which is 794 x 559
	// Hence, scroll bars are shown even though it would not be necessary.

	// With this line, we make sure to round *upwards* to nearest integer!
	// baseSize += QSizeF(0.5, 0.5);

	// baseSize += QSizeF(frameWidth * 2, frameWidth * 2);
	QSize result = baseSize.boundedTo((3 * QApplication::desktop()->size()) / 4).toSize();
	qDebug() << "result  : " << result;

	return result;
}


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	// setup main window
    resize(1024, 768);

    // create the central widget
	QWidget* centralwidget = new QWidget(this);

    // create a QGraphicsScene
	QGraphicsScene* scene = new QGraphicsScene();
    QGraphicsRectItem* item = new QGraphicsRectItem(10, 10, 50, 50);
    scene->addItem(item);
    item = new QGraphicsRectItem(0, 0, 5, 5);
    scene->addItem(item);
    item = new QGraphicsRectItem(225, 295, 5, 5);
    scene->addItem(item);
    item = new QGraphicsRectItem(125, 100, 200, 100);
    scene->addItem(item);

    // create the QGraphicsView
	QGraphicsView* view = new GraphicsView(centralwidget);
    view->setFrameStyle(0);	// do not show a frame around the scroll area
    view->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    view->setObjectName(QStringLiteral("view"));
    view->setScene(scene);

    // create a layout to center the graphics view inside the central widget
    QGridLayout* ml = new QGridLayout(centralwidget);
    ml->setRowStretch(0, 1);
    ml->setMargin(0);
    ml->setHorizontalSpacing(0);
    ml->setVerticalSpacing(0);

    ml->setRowStretch(2, 1);
    ml->setColumnStretch(0, 1);
    ml->setColumnStretch(2, 1);
    ml->addWidget(view, 1, 1);
    centralwidget->setLayout(ml);

    // set the central widget
    setCentralWidget(centralwidget);

    // setup scene size and scaling
	scene->setSceneRect(QRectF(0, 0, 210, 148));

	QTransform transform;
	transform.scale(3.77953 ,  3.77953);
	view->setTransform(transform);
}


int main(int argc, char ** argv) {
    QApplication app( argc, argv );

    MainWindow win(0);
    win.show();
    app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
    return app.exec();
}
