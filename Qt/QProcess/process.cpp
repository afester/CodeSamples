#include <QDebug>
#include <QApplication>
#include <QProcess>
#include <QTimer>

#include "process.h"

int main(int argc, char* argv[]) {
   QCoreApplication app( argc, argv );

   UsageStatistics us;
   app.exec();
 
   return 0;
}

