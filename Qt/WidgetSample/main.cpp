/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

#include <QApplication>
#include "WidgetSample.h"

int main(int argc, char ** argv) {

    QApplication app( argc, argv );

    MainWindow win(0);
    win.show(); 
    QObject::connect( &app, &QApplication::lastWindowClosed, &app, &QApplication::quit );

    return app.exec();
}
