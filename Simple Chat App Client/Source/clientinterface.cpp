#include "clientinterface.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QStatusBar>
#include <QMenu>
#include <QDebug>
#include <QMessageBox>

ClientInterface::ClientInterface(QWidget *parent)
    : QMainWindow(parent)
{
    setInterface();
    clientConnection = new Connection();
    connectionThread = new QThread;

    connect(clientConnection, &Connection::connectionSucceeded, this, &ClientInterface::connectionStatus);
    connect(clientConnection, &Connection::unconnectedState, this, &ClientInterface::showUnconnectedMsgBox);
    connect(this, &ClientInterface::reconnectToServer, clientConnection, &Connection::connectToServer);
    connect(clientConnection->tcpSocket, &QTcpSocket::readyRead, this, &ClientInterface::readMessages);
    connect(sendBtn, &QPushButton::clicked, this, &ClientInterface::sendMessages);
    connect(this, &ClientInterface::writeMessages, clientConnection, &Connection::sendMessages);

    clientConnection->moveToThread(connectionThread);
    connect(connectionThread, &QThread::started, clientConnection, &Connection::connectToServer);
    connect(connectionThread, &QThread::finished, connectionThread, &QThread::deleteLater);
    connect(connectionThread, &QThread::finished, clientConnection, &QObject::deleteLater);
    connectionThread->start();

    connect(userNames, &QListWidget::itemChanged, this, &ClientInterface::changeClientName);

    connect(this, &ClientInterface::onlineUsersFresh, this, &ClientInterface::refreshOnlineUsers);
}

ClientInterface::~ClientInterface()
{

}

void ClientInterface::sendMessages()
{
//    localHostName = QHostInfo::localHostName() + " %1";
    QString string =/* localHostName +*/ inputBox->toPlainText();
    emit writeMessages(string);
    qDebug() << string;
    qDebug() << "Message sent to server";
    QString currentTime = QTime::currentTime().toString("h:mm:ss AP");
    QString sendMessageColor = "<font color = \"blue\">";
    messageBox->insertHtml(sendMessageColor % currentTime % "<br>");
    messageBox->insertHtml(sendMessageColor % inputBox->toPlainText() % "<br>");
}

void ClientInterface::changeUserName()
{
    QListWidgetItem *editItem = userNames->currentItem();
    editItem->setFlags(editItem->flags() | Qt::ItemIsEditable);
    qDebug() << "user name changed";
}

void ClientInterface::showUnconnectedMsgBox()
{
    QMessageBox unconnectedMsgBox;
    unconnectedMsgBox.setIcon(QMessageBox::Question);
    unconnectedMsgBox.setText("Failed to connect to server.");
    unconnectedMsgBox.setInformativeText("Do you want to try again?");
    unconnectedMsgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    unconnectedMsgBox.setDefaultButton(QMessageBox::No);
    unconnectedMsgBox.setMinimumSize(200, 200);
    int ret = unconnectedMsgBox.exec();
    switch (ret) {
    case QMessageBox::Yes:
        emit reconnectToServer();
        break;
    case QMessageBox::No:
        unconnectedMsgBox.close();
        break;
    default:
        break;
    }
}

void ClientInterface::changeClientName(QListWidgetItem *editItem)
{
    localHostName = "01_clientName: " + editItem->text();
    qDebug() << localHostName;
    emit writeMessages(localHostName);
}

void ClientInterface::refreshOnlineUsers()
{
    userNames->clear();
    for(int i = 0; i < allOnlineUsers.size(); i++) {
        userNames->addItem(allOnlineUsers[i]);
    }
}

void ClientInterface::setInterface()
{
    clientInterface = new QWidget(this);
    setCentralWidget(clientInterface);

    messageBox = new QTextEdit(clientInterface);
    messageBox->setMinimumSize(700, 400);
    messageBox->setStyleSheet("border: 1px solid black");
    messageBox->setReadOnly(true);

    localHostName = QHostInfo::localHostName() + " %1";

    userNames = new QListWidget(clientInterface);
    userNames->addItem(localHostName.arg("Offline"));
    userNames->setMaximumWidth(300);
    userNames->setStyleSheet("border: 1px solid black");
    connect(userNames, &QListWidget::itemDoubleClicked, this, &ClientInterface::changeUserName);

    inputBox = new QTextEdit(clientInterface);
    inputBox->setMaximumHeight(200);

    QHBoxLayout *dialogLayout = new QHBoxLayout;
    dialogLayout->addWidget(messageBox);
    dialogLayout->addWidget(userNames);

    sendBtn = new QPushButton(clientInterface);
    sendBtn->setText("Send");

    QSpacerItem *btnSpacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);

    QHBoxLayout *btnsLayout = new QHBoxLayout;
    btnsLayout->addSpacerItem(btnSpacer);
    btnsLayout->addWidget(sendBtn);

    QVBoxLayout *messagesLayout = new QVBoxLayout;
    messagesLayout->addWidget(inputBox);
    messagesLayout->addLayout(btnsLayout);

    QVBoxLayout *chatLayout = new QVBoxLayout(clientInterface);
    clientInterface->setLayout(chatLayout);
    chatLayout->addLayout(dialogLayout);
    chatLayout->addLayout(messagesLayout);

    statusBar()->showMessage("Offline");
}

void ClientInterface::connectionStatus()
{
    statusBar()->showMessage("Online");
    userNames->item(0)->setText(localHostName.arg("Online"));
    qDebug() << "Status changed";
}

void ClientInterface::readMessages()
{
    QDataStream readData(clientConnection->tcpSocket);
    QString fromServer;
    readData >> fromServer;

    QStringList textAndUsernames = fromServer.split("allOnlineUsers: ");
    qDebug() << textAndUsernames;

    QStringList messageAndTime = textAndUsernames[0].split("currentTime: ");
    qDebug() << "message and time: " << messageAndTime;
    QString recievedMessageColor = "<font color = \"green\">";
    messageBox->insertHtml(recievedMessageColor % messageAndTime[1] % "<br>");
    messageBox->insertHtml(recievedMessageColor % messageAndTime[0] % "<br>");

    QStringList allOnlineClients = textAndUsernames[1].split("allOnlineClients;");
    allOnlineUsers.clear();
    for(int i = 0; i < allOnlineClients.size() - 1; i++) {
        allOnlineUsers.push_back(allOnlineClients[i]);
    }
    qDebug() << "all user names: " << allOnlineClients;
    emit onlineUsersFresh();
}
