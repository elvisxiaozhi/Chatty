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

}

void Server::disconnected()
{
    QTcpSocket *socket = static_cast<QTcpSocket *>(sender());
    int pos = std::find(socketVec.begin(), socketVec.end(), socket) - socketVec.begin();
    socketVec[pos]->deleteLater();
    socketVec.erase(socketVec.begin() + pos);
    socket->deleteLater();
}
