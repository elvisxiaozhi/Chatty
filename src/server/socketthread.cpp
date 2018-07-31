#include "socketthread.h"
#include <QDataStream>
#include <QDebug>

SocketThread::SocketThread(qintptr descriptor, QObject *parent)
    : QThread(parent), socketDescriptor(descriptor)
{
    socket = new QTcpSocket();
    socket->setSocketDescriptor(socketDescriptor);

    socket->moveToThread(this);
    this->start();

    connect(socket, &QTcpSocket::readyRead, this, &SocketThread::readyRead, Qt::DirectConnection);
    connect(socket, &QTcpSocket::disconnected, this, &SocketThread::disconnected, Qt::DirectConnection);
}

SocketThread::~SocketThread()
{
    quit();
    wait();
}

void SocketThread::readyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_10);

    QString message;
    in >> message;
    qDebug() << message;

    socket->disconnectFromHost();
}

void SocketThread::disconnected()
{
    socket->close();
    quit();
}
