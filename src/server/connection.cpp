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
    qDebug() << "Ran";
    SocketThread *socketThread = new SocketThread(socketDescriptor);
    connect(socketThread, &SocketThread::finished, socketThread, &QObject::deleteLater);
//    socketThread->start();
}
