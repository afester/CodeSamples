#include <QCoreApplication>
#include <QApplication>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QDebug>

bool createConnection();

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    if (!createConnection())
     {
        qDebug() << "Not connected!";
        return 1;
     }
     else
     {
        qDebug() << "Connected!";
        QSqlQuery query;
        query.exec("SELECT name FROM info");
        while (query.next()) 
        {
            QString name = query.value(0).toString();
            qDebug() << "name:" << name;
        }
        return 0;
    }
    return app.exec();
}

bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("dbname");
    db.setUserName("dbuser");
    db.setPassword("dbpassword");
    if (!db.open()) 
    {
        qDebug() << "Database error occurred";
        return false;
    }
    return true;
}
