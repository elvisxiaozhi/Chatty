#include "socket.h"
#include <QHostAddress>

Socket::Socket(QObject *parent) : QTcpSocket(parent)
{
    connect(this, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Socket::getError);
}

void Socket::connectToServer()
{
    abort();
    connectToHost(QHostAddress("127.0.0.1"), 6666);
    if(!waitForConnected()) {
        qDebug() << "Failed to connect";
    }
}

void Socket::getError(QAbstractSocket::SocketError error)
{
    qDebug() << error;
    emit unconnected();
}
