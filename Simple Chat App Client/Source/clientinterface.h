#ifndef CLIENTINTERFACE_H
#define CLIENTINTERFACE_H

#include <QMainWindow>
#include <QLabel>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QListWidgetItem>
#include "connection.h"
#include <QThread>

class ClientInterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientInterface(QWidget *parent = 0);
    ~ClientInterface();

private:
    QWidget *clientInterface;
    QTextEdit *messageBox;
    QTextEdit *inputBox;
    QPushButton *sendBtn;
    QListWidget *userNames;
    QString localHostName;
    QThread *connectionThread = nullptr;
    Connection *clientConnection = nullptr;

    QVector<QString> allOnlineUsers;

    void setInterface();

signals:
    void writeMessages(QString);
    void reconnectToServer();
    void onlineUsersFresh();

private slots:
    void connectionStatus();
    void readMessages();
    void sendMessages();
    void changeUserName();
    void showUnconnectedMsgBox();
    void changeClientName(QListWidgetItem *);
    void refreshOnlineUsers();
};

#endif // CLIENTINTERFACE_H
