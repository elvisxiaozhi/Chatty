#include "socketthread.h"
#include <QDebug>

SocketThread::SocketThread(qintptr descriptor, QObject *parent)
    : QThread(parent), socketDescriptor(descriptor)
{
    socket = new QTcpSocket();
    socket->setSocketDescriptor(socketDescriptor);

    socket->moveToThread(this);
    this->start();

    connect(socket, &QTcpSocket::readyRead, this, &SocketThread::readMessage, Qt::DirectConnection);
    connect(socket, &QTcpSocket::disconnected, this, &SocketThread::disconnected, Qt::DirectConnection);
}

SocketThread::~SocketThread()
{
    quit();
    wait();
}

void SocketThread::writeToClient(QString message)
{
    socket->write(message.toUtf8());
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
