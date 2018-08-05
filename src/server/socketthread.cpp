#include "socketthread.h"
#include <QDebug>

SocketThread::SocketThread(qintptr descriptor, QObject *parent)
    : QThread(parent), socketDescriptor(descriptor)
{
    socket = new QTcpSocket();
    socket->setSocketDescriptor(socketDescriptor);

    socket->moveToThread(this);

    connect(socket, &QTcpSocket::readyRead, this, &SocketThread::readMessage);
    connect(socket, &QTcpSocket::disconnected, this, &SocketThread::disconnected);
}

SocketThread::~SocketThread()
{
    quit();
    wait();
}

void SocketThread::readMessage()
{
    QString message = socket->readAll();
    qDebug() << "From client" << socket->socketDescriptor() << message;
}

void SocketThread::disconnected()
{
    socket->close();
    quit();
}
