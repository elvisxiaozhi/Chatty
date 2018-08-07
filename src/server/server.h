#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include "socketthread.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    void startServer();

private:
    QVector<SocketThread *> threadVec;
    QString returnSocketName();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

};

#endif
