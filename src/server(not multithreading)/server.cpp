#include "server.h"

Server::Server(QObject *parent) : QTcpServer(parent)
{
    connect(this, &Server::newConnection, this, &Server::clientConnected);

}

void Server::startServer()
{
    if(!listen(QHostAddress::LocalHost, 6666)) {
        qDebug() << "Could not start server";
    }
}

void Server::clientConnected()
{
    QTcpSocket *socket = nextPendingConnection();

    socketVec.push_back(socket);

    int socketDescriptor = socket->socketDescriptor();
    qDebug() << socketDescriptor << "connected.";

    connect(socket, &QTcpSocket::readyRead, this, &Server::readyRead);
    connect(socket, &QTcpSocket::disconnected, [this, socketDescriptor]() {
        qDebug() << socketDescriptor << "disconnected";
        for(int i = 0; i < socketVec.size(); ++i) {
            socketVec[i]->write("disconnectedSocket: " + QString::number(socketDescriptor).toUtf8());
        }
    });
    connect(socket, &QTcpSocket::disconnected, this, &Server::disconnected);
}

void Server::readyRead()
{
    socketSender = static_cast<QTcpSocket *>(sender());

    QString message = socketSender->readAll();

    if(message.contains("localHostName: ")) {
        usernameVec.push_back(message.split("localHostName: ").at(1));

        for(int i = 0; i < socketVec.size(); ++i) {
            QString message;
            for(int j = 0; j < usernameVec.size(); ++j) {
                if(j != i) {
                    message.append(usernameVec[j] + " " + QString::number(socketVec[j]->socketDescriptor()) + " NextSocket: ");
                }
            }
            socketVec[i]->write(message.toUtf8());
        }
    }
    if(message.contains("messageSendTo: ")) {
        QStringList stringList = QString(message.split("messageSendTo: ").at(1)).split(" hereAreMessages: ");
        for(int i = 0; i < socketVec.size(); ++i) {
            if(socketVec[i]->socketDescriptor() == stringList[0].toInt()) {
                socketVec[i]->write("messageFrom: " + QString::number(socketSender->socketDescriptor()).toUtf8() + " hereAreMessages: " + QString(stringList[1]).toUtf8());
            }
        }
    }
}

void Server::disconnected()
{
    socketSender = static_cast<QTcpSocket *>(sender());
    int pos = std::find(socketVec.begin(), socketVec.end(), socketSender) - socketVec.begin();
    socketVec[pos]->deleteLater();
    socketVec.erase(socketVec.begin() + pos);
    usernameVec.erase(usernameVec.begin() + pos);
    socketSender->deleteLater();
}
