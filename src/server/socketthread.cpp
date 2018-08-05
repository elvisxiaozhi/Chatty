#include "socketthread.h"

SocketThread::SocketThread(qintptr ID, QObject *parent) :
    QThread(parent)
{
    socketDescriptor = ID;
}

SocketThread::~SocketThread()
{
}

void SocketThread::run()
{
    // thread starts here

    socket = new QTcpSocket();

    // set the ID
    if(!socket->setSocketDescriptor(socketDescriptor)) {
        // something's wrong, we just emit a signal
        emit error(socket->error());
        return;
    }

    // note - Qt::DirectConnection is used because it's multithreaded
    //        This makes the slot to be invoked immediately, when the signal is emitted.
    connect(socket, &QTcpSocket::readyRead, this, &SocketThread::readyRead, Qt::DirectConnection);
    connect(socket, &QTcpSocket::disconnected, this, &SocketThread::disconnected);

    // We'll have multiple clients, we want to know which is which
    qDebug() << socketDescriptor << " Client connected";

    // make this thread a loop,
    // thread will stay alive so that signal/slot to function properly
    // not dropped out in the middle when thread dies

    socket->write("Hello");

    exec();
}

void SocketThread::readyRead()
{
    // get the information
    QByteArray Data = socket->readAll();

    // will write on server side window
    qDebug() << socketDescriptor << " Data in: " << Data;

//    socket->write(Data);
}

void SocketThread::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";

    socket->deleteLater();
    quit();
}
