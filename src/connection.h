#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>

class Connection : public QTcpSocket
{
    Q_OBJECT
public:
    explicit Connection(QObject *parent = nullptr);
    void connectToServer();

signals:
    void connected();
    void unconnected();

private slots:
    void getError(QAbstractSocket::SocketError);
};

#endif // CONNECTION_H
