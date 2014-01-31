
#include <QApplication>
#include <QMainWindow>
#include <QPainter>

#include "ui_mainwindow.h"
#include <stdio.h>
#include "main.h"



class TestMainWindow : public Ui::MainWindow {
public:
	virtual ~TestMainWindow() {}

	virtual void setupUi(QMainWindow* parent)  {
		Ui::MainWindow::setupUi(parent);
		for (int i = 0;  i < 20;  i++) {
			QPushButton* b = new QPushButton("Button", verticalLayoutWidget);
			verticalLayout->addWidget(b);
		}
		verticalLayout->activate();
		scrollAreaWidgetContents->setMinimumHeight(verticalLayout->geometry().height());
	}
};

 static const QPoint points[5] = {
     QPoint(0, 0),
     QPoint(399, 0),
     QPoint(398, 98),
     QPoint(50, 98),
     QPoint(50, 20)
 };

class MessageBubble : public QWidget {
public:
	MessageBubble(QWidget* parent) : QWidget(parent, Qt::Popup | Qt::Window) {
		QPolygon poly;
		for (int i = 0;  i < 5; i++) {
			poly << points[i];
		}
		QRegion reg(poly);
		setMask(reg);
	}

protected:
	virtual void paintEvent ( QPaintEvent * event) {
		QPainter painter(this);
		painter.setBrush(QBrush(Qt::green));
		painter.drawPolygon(points, 5);
	}
};


TheMainWindow::TheMainWindow() {
	mainUi = new TestMainWindow();
	mainUi->setupUi(this);
	connect(mainUi->pushButton, SIGNAL(clicked()), this, SLOT(toggleWidget()));

	messageBox = new MessageBubble(this);
	messageBox->setAutoFillBackground(false);
	messageBox->setGeometry(100, 100, 400, 100);
}

TheMainWindow::~TheMainWindow() {
}

void TheMainWindow::toggleWidget() {
	if (mainUi->pushButton->isChecked()) {
		mainUi->pushButton_2->setText("Hello");
		messageBox->move(mainUi->pushButton->x() + mainUi->pushButton->width(),
				 mainUi->pushButton->y() + mainUi->pushButton->height());
		messageBox->show();
	} else {
		mainUi->pushButton_2->setText("World");
		messageBox->hide();
	}
}


int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    TheMainWindow w;
    w.show();

    return app.exec();
}
