#include "connection.h"
#include <QDebug>
#include <QMessageBox>

Connection::Connection(QObject *parent) : QObject(parent)
{
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Connection::getError);
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
        qDebug() << "Failed to connect to server.";
    }
}

void Connection::sendMessages(QString string)
{
    tcpSocket->write(string.toUtf8());
}

void Connection::getError(QAbstractSocket::SocketError error)
{
    qDebug() << "error: " << error;
    if(error == 0 || error == 1) {
        emit unconnectedState();
    }
}

void Connection::disconnectFromServer()
{
    tcpSocket->disconnectFromHost();
    qDebug() << "Disconnected from server";
    connectToServer();
}
