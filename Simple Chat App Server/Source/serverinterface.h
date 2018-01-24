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
    int previousSocketDescriptor;
    int onlineClientsNum;

    void setBasicLayout();

signals:
    void messagesRead();
    void onlineClientsNumChanged(int);

private slots:
    void newClientConnected();
    void clientDisconnected();
    void readMessages();
    void sendMessages();
    void clearSocketDescriptor();
    void changeOnlineClientsNum(int);
};

#endif // SERVERINTERFACE_H
