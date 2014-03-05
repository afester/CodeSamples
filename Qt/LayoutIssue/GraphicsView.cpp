#include <QApplication>
#include <QMainWindow>
#include <QGridLayout>
#include <QScrollArea>

class ColoredWidget : public QWidget {
public:
	ColoredWidget(const  QColor& color, QWidget* parent) : QWidget(parent) {
		QPalette pal;
		QBrush brush(color);
		brush.setStyle(Qt::SolidPattern);
		pal.setBrush(QPalette::Active, QPalette::Window, brush);
		setPalette(pal);
		setAutoFillBackground(true);
	}
};

class MainWindow : public QMainWindow {
public:
	MainWindow(QWidget* parent) : QMainWindow(parent) {
		resize(300, 400);

		QWidget* centralwidget = new ColoredWidget(QColor(0xff, 0xf0, 0xb5), this);

		QGridLayout* layout = new QGridLayout();
		centralwidget->setLayout(layout);
		setCentralWidget(centralwidget);

		// create widget with fixed height of 20 px and maximum width of 200
		QWidget* topHeader = new ColoredWidget(Qt::green, centralwidget);
		topHeader->setMaximumWidth(200);
		topHeader->setFixedHeight(20);

		// create widget with fixed width of 20 px and maximum height of 200
		QWidget* leftHeader = new ColoredWidget(Qt::blue, centralwidget);
		leftHeader->setFixedWidth(20);
//		leftHeader->setMaximumHeight(200);
		leftHeader->setFixedHeight(20);

		// create scroll area as main widget
		QWidget* view = new QScrollArea(centralwidget);
/**********************/
		QPalette pal;
		QBrush brush(Qt::red);
		brush.setStyle(Qt::SolidPattern);
		pal.setBrush(QPalette::Active, QPalette::Window, brush);
		view->setPalette(pal);
		view->setAutoFillBackground(true);
/*****************/
		layout->addWidget(topHeader,  0, 1);
		layout->addWidget(leftHeader, 1, 0);
		// layout->addWidget(leftHeader, 1, 0, Qt::AlignTop); // widget not displayed at all!
		layout->addWidget(view, 1, 1);
	}
};


int main(int argc, char ** argv) {
    QApplication app( argc, argv );
    MainWindow win(0);
    win.show();
    return app.exec();
}
