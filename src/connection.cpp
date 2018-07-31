#include "connection.h"
#include <QHostAddress>
#include <QDebug>
#include <QAbstractSocket>

Connection::Connection(QObject *parent) : QTcpSocket(parent)
{
//    connect(this, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Connection::getError);
}

void Connection::connect()
{
    abort();
    connectToHost(QHostAddress("127.0.0.1"), 6666);
    if(waitForConnected()) {
        emit connected();
    }
    else {
        qDebug() << "Failed to connect";
    }
}

void Connection::getError(QAbstractSocket::SocketError error)
{
    qDebug() << error;
}
