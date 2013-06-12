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
    Canvas* canvas = new Canvas(ui->centralwidget, 320, 240);
    ui->verticalLayout->addWidget(canvas);

    canvas->drawLine(10, 10, 100, 100);
}

MainWindow::~MainWindow() {
    delete ui;
}
