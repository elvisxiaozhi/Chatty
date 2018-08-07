#include "connection.h"
#include <QHostAddress>
#include <QDebug>

Connection::Connection(QObject *parent) : QTcpSocket(parent)
{
    connect(this, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Connection::getError);
}

void Connection::connectToServer()
{
    abort();
    connectToHost(QHostAddress("127.0.0.1"), 6666);
    if(!waitForConnected()) {
        qDebug() << "Failed to connect";
    }
}

void Connection::getError(QAbstractSocket::SocketError error)
{
    qDebug() << error;
    emit unconnected();
}
