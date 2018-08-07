#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QDebug>
#include <QTcpSocket>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    void startServer();

private:
    QVector<QTcpSocket *> socketVec;

private slots:
    void clientConnected();
    void readyRead();
    void disconnected();
};

#endif // SERVER_H
