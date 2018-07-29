#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpServer>

class Connection : public QObject
{
    Q_OBJECT
public:
    Connection();

private:
    QTcpServer *server;
};

#endif // CONNECTION_H
