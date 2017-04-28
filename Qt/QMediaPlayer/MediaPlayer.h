/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */


#include <QMainWindow>
#include <QMediaPlayer>
#include <QAbstractSocket>

class QTcpSocket;
class QPushButton;

class MainWindow : public QMainWindow {
    Q_OBJECT;

    QTcpSocket* tcpSocket;
    QPushButton* connectButton;
    QPushButton* startPlayButton;
    QPushButton* stopPlayButton;
    QPushButton* streamButton;
    QMediaPlayer* player;
    QString hostName;

public:

    MainWindow(QWidget* parent, const QString &theHostName);

    ~MainWindow();

public slots:
    virtual void statusChanged(QMediaPlayer::MediaStatus status);
    virtual void doConnect();
    virtual void doStartPlay();
    virtual void doStopPlay();
    virtual void doStream();
    virtual void socketError(QAbstractSocket::SocketError socketError);
    virtual void socketConnected();
    virtual void socketDisconnected();
    virtual void readData();
};
