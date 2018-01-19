#include "connection.h"

Connection::Connection(QObject *parent) : QObject(parent)
{
    tcpServer = new QTcpServer(this);
    tcpServer->listen(QHostAddress::Any, 6666);
}
