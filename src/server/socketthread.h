#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>

class SocketThread : public QThread
{
    Q_OBJECT
public:
    SocketThread(qintptr descriptor, QObject *parent = 0);
    ~SocketThread();
    qintptr socketDescriptor;

private:
    QTcpSocket *socket;

signals:
    void clientDisconnected();

private slots:
    void readMessage();
    void disconnected();
};

#endif // SOCKETTHREAD_H
