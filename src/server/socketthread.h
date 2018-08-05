#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>

class SocketThread : public QThread
{
    Q_OBJECT
public:
    SocketThread(qintptr ID, QObject *parent = 0);
    ~SocketThread();
    qintptr socketDescriptor;
    void run();

signals:
    void error(QTcpSocket::SocketError socketerror);

public slots:
    void readyRead();
    void disconnected();

private:
    QTcpSocket *socket;
};

#endif
