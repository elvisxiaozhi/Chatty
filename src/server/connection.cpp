#include "connection.h"
#include <QHostAddress>

Connection::Connection()
{
    server = new QTcpServer();
    server->listen(QHostAddress::Any, 6666);
}
