#include "server.h"

Server::Server(QObject *parent) : QTcpServer(parent)
{
    connect(this, &Server::newConnection, this, &Server::clientConnected);

}

void Server::startServer()
{
    if(!listen(QHostAddress::Any, 6666)) {
        qDebug() << "Could not start server";
    }
}

void Server::clientConnected()
{
    QTcpSocket *socket = nextPendingConnection();

    for(int i = 0; i < socketVec.size(); ++i) {
        ;
    }

    socketVec.push_back(socket);

    int socketDescriptor = socket->socketDescriptor();
    qDebug() << socketDescriptor << "connected.";

    connect(socket, &QTcpSocket::readyRead, this, &Server::readyRead);
    connect(socket, &QTcpSocket::disconnected, [this, socketDescriptor]() {
        qDebug() << socketDescriptor << "disconnected";
    });
    connect(socket, &QTcpSocket::disconnected, this, &Server::disconnected);
}

void Server::readyRead()
{
    socketSender = static_cast<QTcpSocket *>(sender());

    qDebug() << socketSender->socketDescriptor() << socketSender->readAll();
}

void Server::disconnected()
{
    socketSender = static_cast<QTcpSocket *>(sender());
    int pos = std::find(socketVec.begin(), socketVec.end(), socketSender) - socketVec.begin();
    socketVec[pos]->deleteLater();
    socketVec.erase(socketVec.begin() + pos);
    socketSender->deleteLater();
}
