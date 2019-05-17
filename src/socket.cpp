#include "socket.h"
#include <QHostAddress>

Socket::Socket(QObject *parent) : QTcpSocket(parent)
{
    connect(this, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Socket::getError);
}

void Socket::connectToServer()
{
    abort();
    connectToHost(QHostAddress("34.80.123.141"), 6666);
    if(!waitForConnected()) {
        qDebug() << "Failed to connect";
    }
}

void Socket::getError(QAbstractSocket::SocketError error)
{
    qDebug() << error;
    emit unconnected();
}
