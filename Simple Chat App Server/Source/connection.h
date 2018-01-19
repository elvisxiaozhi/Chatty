#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QtNetwork>

class Connection : public QObject
{
    Q_OBJECT
public:
    explicit Connection(QObject *parent = nullptr);
    QTcpServer *tcpServer;

signals:

public slots:
};

#endif // CONNECTION_H
