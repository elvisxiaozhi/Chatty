#ifndef SERVERINTERFACE_H
#define SERVERINTERFACE_H

#include <QMainWindow>
#include <QLabel>
#include <QListWidget>
#include <QTextEdit>
#include <QPushButton>
#include "connection.h"

class ServerInterface : public QMainWindow
{
    Q_OBJECT

public:
    ServerInterface(QWidget *parent = 0);
    ~ServerInterface();

private:
    QWidget *basicInterface;
    QLabel *recievedMessages;
    QListWidget *clientsNames;
    QTextEdit *inputBox;
    QPushButton *sendBtn;
    QVector<QTcpSocket*> connectedClients;
    Connection serverConnection;
    void setBasicLayout();

    int previousSocketDescriptor;

signals:
    void messagesRead();

private slots:
    void newClientConnected();
    void clientDisconnected();
    void readMessages();
    void sendMessages();

    void clearSocketDescriptor();
};

#endif // SERVERINTERFACE_H
