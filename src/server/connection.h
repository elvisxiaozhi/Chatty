#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpServer>

class Connection : public QTcpServer
{
    Q_OBJECT
public:
    explicit Connection(QObject *parent = nullptr);
    ~Connection();

private:

private slots:
    void incomingConnection(qintptr);
};

#endif // CONNECTION_H
