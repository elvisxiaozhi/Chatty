#include "connection.h"
#include <QHostAddress>
#include <QDebug>

Connection::Connection(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
}

void Connection::connect()
{
    socket->abort();
    socket->connectToHost(QHostAddress("127.0.0.1"), 6666);
    if(socket->waitForConnected()) {
        connected();
    }
    else {
        qDebug() << "Failed to connect";
    }
}

void Connection::sendMessage(QString message)
{
    socket->write(message.toUtf8());
}
