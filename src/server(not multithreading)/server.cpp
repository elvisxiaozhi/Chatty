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

    QString message;
    for(int i = 0; i < usernameVec.size(); ++i) {
        message.append(usernameVec[i] + " onlineUser: " + QString::number(socketVec[i]->socketDescriptor()).toUtf8());
    }

    if(message.size() > 0) {
        socket->write(message.toUtf8());
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

    QString message = socketSender->readAll();

    if(message.contains("localHostName: ")) {
        usernameVec.push_back(message.split("localHostName: ").at(1));
    }
    if(message.contains("messageSendTo: ")) {
        QStringList stringList = QString(message.split("messageSendTo: ").at(1)).split("hereAreMessages: ");
        qDebug() << stringList << stringList[0].toInt();
        for(int i = 0; i < socketVec.size(); ++i) {
            if(socketVec[i]->socketDescriptor() == stringList[0].toInt()) {
                socketVec[i]->write(QString(stringList[1]).toUtf8());
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
