/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported 
 * License. To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative 
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

#include <QDebug>
#include <QLayout>
#include <QLabel>
#include <QKeyEvent>
#include <QPainter>
#include <QPushButton>
#include <QVideoWidget>
#include <QMediaPlaylist>
#include <QMediaPlayer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTcpSocket>


#include "MediaPlayer.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    setObjectName(QStringLiteral("MainWindow"));
    resize(400, 300);


//    QMediaPlaylist* playlist = new QMediaPlaylist;
//    playlist->addMedia(QUrl("file:///SampleVideo.mp4"));

    player = new QMediaPlayer;
//    player->setPlaylist(playlist);

    QWidget* centralWidget = new QWidget(this);
    QBoxLayout* mainLayout = new QVBoxLayout();
    centralWidget->setLayout(mainLayout);

    QWidget* buttonArea = new QWidget(this);
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonArea->setLayout(buttonLayout);

    QVideoWidget* videoWidget = new QVideoWidget(centralWidget);
    mainLayout->addWidget(videoWidget, 1);
    mainLayout->addWidget(buttonArea);

    connectButton = new QPushButton("Connect", buttonArea);
    startPlayButton = new QPushButton("Play", buttonArea);
    startPlayButton->setEnabled(false);
    stopPlayButton = new QPushButton("Stop", buttonArea);
    stopPlayButton->setEnabled(false);
    buttonLayout->addWidget(connectButton);
    buttonLayout->addWidget(startPlayButton);
    buttonLayout->addWidget(stopPlayButton);

    tcpSocket = new QTcpSocket(this);

    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            this,   SLOT(statusChanged(QMediaPlayer::MediaStatus)));
    connect(connectButton, SIGNAL(clicked()),
            this,   SLOT(doConnect()));
    connect(startPlayButton, SIGNAL(clicked()),
            this,   SLOT(doStartPlay()));
    connect(stopPlayButton, SIGNAL(clicked()),
            this,   SLOT(doStopPlay()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this,      SLOT(socketError(QAbstractSocket::SocketError)));
    connect(tcpSocket, SIGNAL(connected()),
            this,      SLOT(socketConnected()));
    connect(tcpSocket, SIGNAL(disconnected()),
            this,      SLOT(socketDisconnected()));
    connect(tcpSocket, SIGNAL(readyRead()),
            this,      SLOT(readData()));
    player->setVideoOutput(videoWidget);

    setCentralWidget(centralWidget);
//    player->play();
}


MainWindow::~MainWindow() {
}

void MainWindow::statusChanged(QMediaPlayer::MediaStatus status) {
    qDebug() << "STATUS CHANGED: " << status;
}

void MainWindow::doConnect() {
    qDebug() << "doConnect";

    connectButton->setEnabled(false);

    if (tcpSocket->state() == QAbstractSocket::ConnectedState) {
        tcpSocket->abort();
        tcpSocket->disconnectFromHost();
    } else {
        tcpSocket->abort();
        tcpSocket->connectToHost("server", 1234);
    }
}


void MainWindow::socketConnected() {
    qDebug() << "Connected.";
    startPlayButton->setEnabled(true);
    stopPlayButton->setEnabled(true);
    connectButton->setText("Disconnect");
    connectButton->setEnabled(true);
}


void MainWindow::socketDisconnected() {
    qDebug() << "Disconnected.";
    startPlayButton->setEnabled(false);
    stopPlayButton->setEnabled(false);
    connectButton->setText("Connect");
    connectButton->setEnabled(true);
}

void MainWindow::socketError(QAbstractSocket::SocketError socketError) {
    qDebug() << "Socket error: " << socketError;
}

static bool playing = false;

void MainWindow::readData() {
qDebug() << "Data ready.";
if (!playing) {
    playing = true;
    player->setMedia(QMediaContent(), tcpSocket);
    player->play();
}
//    char buffer[2048];
//    qint64 numbytes = tcpSocket->read(buffer, 2048);
//    qDebug() << numbytes << " received.";
}

void MainWindow::doStartPlay() {
    qDebug() << "doStartPlay";

//    qDebug() << player->media().resources().length();
//    qDebug() << player->media().resources().at(0).videoCodec();
//    qDebug() << player->media().resources().at(0).videoBitRate();


    QString command = "START\n";
    tcpSocket->write(command.toUtf8().data());

    //QMediaResource mr;
//    QMediaContent mc(mr);

}


void MainWindow::doStopPlay() {
    qDebug() << "doStopPlay";
//    player->setMedia(QMediaContent(), tcpSocket);
    //player->play();
    QString command = "STOP\n";
    tcpSocket->write(command.toUtf8().data());
}
