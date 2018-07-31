#include "connection.h"
#include <QHostAddress>
#include <QTcpSocket>
#include <QDebug>
#include "socketthread.h"

Connection::Connection(QObject *parent) : QTcpServer(parent)
{
    this->listen(QHostAddress::Any, 6666);
}

Connection::~Connection()
{
}

void Connection::incomingConnection(qintptr socketDescriptor)
{
    SocketThread *socketThread = new SocketThread(socketDescriptor);
    connect(socketThread, &SocketThread::finished, [this, &socketThread, socketDescriptor]() {
        qDebug() << "Client" << socketDescriptor << "disconnected.";
        delete socketThread;
    });
    qDebug() << "Client" << socketDescriptor << "connected.";
}
