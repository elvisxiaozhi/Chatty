#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>

class Socket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit Socket(QObject *parent = nullptr);
    void connectToServer();

signals:
    void unconnected();

private slots:
    void getError(QAbstractSocket::SocketError);
};

#endif
