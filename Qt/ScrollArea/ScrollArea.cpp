/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

#include <iostream>
#include <typeinfo>

#include <QDebug>

#include "ScrollArea.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent) {

    setObjectName(QStringLiteral("MainWindow"));
    resize(1024, 768);
}


void MainWindow::printInfo() {
	qDebug() << "\nLAYOUT INFO:\n====================";
}


MainWindow::~MainWindow() {
}
