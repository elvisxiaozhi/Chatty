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
    connect(this, &ClientInterface::writeMessages, clientConnection, &Connection::sendMessages);
    connect(sendBtn, &QPushButton::clicked, this, &ClientInterface::sendMessages);

    clientConnection->moveToThread(connectionThread);
    connect(connectionThread, &QThread::started, clientConnection, &Connection::connectToServer);
    connect(connectionThread, &QThread::finished, connectionThread, &QThread::deleteLater);
    connect(connectionThread, &QThread::finished, clientConnection, &QObject::deleteLater);
    connectionThread->start();
}

ClientInterface::~ClientInterface()
{

}

void ClientInterface::sendMessages()
{
    QString string = localHostName + inputBox->toPlainText();
    emit writeMessages(string);
    qDebug() << string;
    qDebug() << "Message sent to server";
}

void ClientInterface::changeUserName()
{
    QListWidgetItem *editItem = userNames->currentItem();
    editItem->setFlags(editItem->flags() | Qt::ItemIsEditable);
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

void ClientInterface::displayError(QAbstractSocket::SocketError)
{
    qDebug() << clientConnection->tcpSocket->error();
    qDebug() << clientConnection->tcpSocket->errorString();
}

void ClientInterface::setInterface()
{
    clientInterface = new QWidget(this);
    setCentralWidget(clientInterface);

    messageBox = new QPlainTextEdit(clientInterface);
    messageBox->setMinimumSize(700, 400);
    messageBox->setStyleSheet("border: 1px solid black");

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
    QStringList timeAndMessages = fromServer.split("currentTime:");
    qDebug() << timeAndMessages;
    messageBox->appendPlainText(timeAndMessages[1]);
    messageBox->appendPlainText(timeAndMessages[0]);
}
