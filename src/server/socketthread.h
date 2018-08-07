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
    QString message;

    void run() override;

private:
    QTcpSocket *socket;

signals:
    void error(QTcpSocket::SocketError socketerror);

private slots:
    void readyRead();
    void disconnected();
};

#endif
