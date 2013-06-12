/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

#include "ui_Canvas.h"
#include "MainWindow.h"
#include "Canvas.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Add a simple drawing canvas to the main window
    canvas = new Canvas(ui->centralwidget, 320, 240);
    ui->verticalLayout->addWidget(canvas);

    QObject::connect(ui->actionLine, SIGNAL(triggered()), this, SLOT(actionLine()));
    QObject::connect(ui->actionCircle, SIGNAL(triggered()), this, SLOT(actionCircle()));
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::actionLine() {
    canvas->drawLine(10, 10, 100, 100);
}

void MainWindow::actionCircle() {
    canvas->drawEllipse(100, 100, 50, 50);
}
