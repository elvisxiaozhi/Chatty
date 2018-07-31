#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpSocket>

class Connection : public QObject
{
    Q_OBJECT
public:
    explicit Connection(QObject *parent = nullptr);
    void connect();
    void sendMessage(QString);

private:
    QTcpSocket *socket;

signals:
    void connected();
};

#endif // CONNECTION_H
