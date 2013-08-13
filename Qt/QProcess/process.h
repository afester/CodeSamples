#include <QDebug>
#include <QApplication>
#include <QProcess>
#include <QTimer>


class UsageStatistics : public QObject {
    Q_OBJECT 

    QProcess* process;
    QTimer* timer;

public:
    UsageStatistics() : process(new QProcess) {
       timer = new QTimer(this);
       connect(timer, SIGNAL(timeout()), this, SLOT(getMemoryUsage()));
       timer->start(100); // one second
    }

    virtual ~UsageStatistics() {}

public slots:

    void getMemoryUsage() {
        process->start("/usr/bin/free");
        if (!process->waitForFinished()) {
            // error processing
        }

         QByteArray result = process->readAll();
        QString line (result.left(40));
        qDebug() << line;
        // parse result 

    }
};
