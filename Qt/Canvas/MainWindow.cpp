/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QDebug>
#include <QScreen>

#include "ui_Canvas.h"
#include "MainWindow.h"
#include "Canvas.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Add a simple drawing canvas to the main window
    canvas = new Canvas(ui->centralwidget, 320, 240);
    ui->verticalLayout->addWidget(canvas);

    QObject::connect(ui->actionPrint, SIGNAL(triggered()), this, SLOT(actionPrint()));
    QObject::connect(ui->actionLine, SIGNAL(triggered()), this, SLOT(actionLine()));
    QObject::connect(ui->actionLine2, SIGNAL(triggered()), this, SLOT(actionLine()));
    QObject::connect(ui->actionCircle, SIGNAL(triggered()), this, SLOT(actionCircle()));
    QObject::connect(ui->actionCircle2, SIGNAL(triggered()), this, SLOT(actionCircle()));
    // QObject::connect(ui->actionRectangle, SIGNAL(triggered()), this, SLOT(actionRectangle()));
    QObject::connect(ui->actionRectangle2, SIGNAL(triggered()), this, SLOT(actionRectangle()));

}

MainWindow::~MainWindow() {
    delete ui;
}
#include <Windows.h>


void MainWindow::actionPrint() {
    // dump screen properties

    SetProcessDPIAware();
    QGuiApplication* gApp = dynamic_cast<QGuiApplication*>(QGuiApplication::instance());
    QList<QScreen*> screens = gApp->screens();

    int num = 1;
    for (QList<QScreen*>::iterator iterScn = screens.begin();
         iterScn != screens.end();
         iterScn++) {
        QScreen* screen = *iterScn;
        qDebug() << num << ". " << screen->name();
        qDebug() << "             Size:" << screen->size();
        qDebug() << "     Virtual Size:" << screen->virtualSize();
        qDebug() << "    Physical Size:" << screen->physicalSize() << " mm"; //!! Basis for physical screen resolution
        qDebug() << "      Logical Dpi:" << screen->logicalDotsPerInchX() << "/" << screen->logicalDotsPerInchY();
        qDebug() << "     Physical Dpi:" << screen->physicalDotsPerInchX() << "/" << screen->physicalDotsPerInchY();
        qDebug() << "            Depth:" << screen->depth();
        qDebug() << "Device Pixel Ratio:" << screen->devicePixelRatio();

        num++;
    }


//    QPrinter printer(QPrinter::HighResolution);
//    printer.setOutputFileName("print.ps");

    QPrinter printer;

    QPrintDialog dialog(&printer, this);
    dialog.setWindowTitle(tr("Print Document"));
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    printer.setFullPage(true);
    QPainter painter;
    painter.begin(&printer);

    qDebug() << "Xres phys:" << printer.physicalDpiX();
    qDebug() << "Yres phys:" << printer.physicalDpiY();

    qDebug() << "Xres log:" << printer.logicalDpiX();
    qDebug() << "Yres log:" << printer.logicalDpiY();

    //float xdpmm = printer.paperRect().width() / printer.pageSizeMM().width() / 10.0;
    //float ydpmm = printer.paperRect().height() / printer.pageSizeMM().height() / 10.0;
    float xdpmm = printer.logicalDpiX() / 254.0;
    float ydpmm = printer.logicalDpiY() / 254.0;
    qDebug() << "X dpmm:" << xdpmm;
    qDebug() << "Y dpmm:" << ydpmm;

    QSizeF pageSize = printer.pageSizeMM();
    QPrinter::PaperSize pSize = printer.paperSize();
    // QSizeF paperSize = printer.pap .paperSize().pageSizeMM();
    qDebug() << "Page size: " << pageSize;
    qDebug() << "Page Rect: " << printer.pageRect();
    qDebug() << "Paper size: " << pSize;    // A4 / Letter
    qDebug() << "Paper Rect: " << printer.paperRect();


    painter.scale(xdpmm, ydpmm);
    qDebug() << "Paper Rect: " << printer.pageRect();

    // scale - "1" should be 0.1mm
    painter.drawRect(100, 100, 1000, 1000);

    painter.end();
}

void MainWindow::actionLine() {
    canvas->drawLine(10, 10, 100, 100);
}

void MainWindow::actionCircle() {
    canvas->drawEllipse(100, 100, 50, 50);
}

void MainWindow::actionRectangle() {
    canvas->drawRectangle(10, 10, 100, 100);
}
