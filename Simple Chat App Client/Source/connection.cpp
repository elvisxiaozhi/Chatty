#include "connection.h"
#include <QDebug>
#include <QMessageBox>

Connection::Connection(QObject *parent) : QObject(parent)
{
    tcpSocket = new QTcpSocket(this);
//    connect(tcpSocket, &QTcpSocket::error, this, &Connection::showError);
}

void Connection::connectToServer()
{
    tcpSocket->abort();
    tcpSocket->connectToHost(QHostAddress("127.0.0.1"), 6666);
    qDebug() << "Connecting to server";
    if(tcpSocket->waitForConnected()) {
        emit connectionSucceeded();
        qDebug() << "Connected to server";
    }
    else {
        emit unconnectedState();
        qDebug() << "Failed to connect to server.";
    }
}

void Connection::sendMessages(QString string)
{
    tcpSocket->write(string.toUtf8());
}

void Connection::showError(QAbstractSocket::SocketError)
{
//    qDebug() << tcpSocket->error();
}
