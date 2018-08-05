#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpServer>
#include "socketthread.h"

class Connection : public QTcpServer
{
    Q_OBJECT
public:
    explicit Connection(QObject *parent = nullptr);
    ~Connection();

private:
    QVector<SocketThread *> threadVec;

private slots:
    void incomingConnection(qintptr);
    QString clientConnected();
};

#endif // CONNECTION_H
