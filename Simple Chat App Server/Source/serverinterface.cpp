#include "serverinterface.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>

ServerInterface::ServerInterface(QWidget *parent)
    : QMainWindow(parent)
{
    setBasicLayout();
    connect(serverConnection.tcpServer, &QTcpServer::newConnection, this, &ServerInterface::newClientConnected);
    connect(sendBtn, &QPushButton::clicked, this, &ServerInterface::clearSocketDescriptor);
    connect(sendBtn, &QPushButton::clicked, this, &ServerInterface::sendMessages);
    connect(this, &ServerInterface::messagesRead, this, &ServerInterface::sendMessages);
}

ServerInterface::~ServerInterface()
{
}

void ServerInterface::setBasicLayout()
{
    basicInterface = new QWidget(this);
    setCentralWidget(basicInterface);

    recievedMessages = new QLabel(basicInterface);
    recievedMessages->setText("No Messages Yet");
    recievedMessages->setAlignment(Qt::AlignCenter);
    recievedMessages->setStyleSheet("border: 1px solid black");
    recievedMessages->setMinimumSize(400, 300);

    clientsNames = new QListWidget(basicInterface);
    clientsNames->addItem("No server");
    clientsNames->setMaximumWidth(300);
    clientsNames->setStyleSheet("border: 1px solid black");

    QHBoxLayout *messageLayout = new QHBoxLayout;
    messageLayout->addWidget(recievedMessages);
    messageLayout->addWidget(clientsNames);

    inputBox = new QTextEdit(basicInterface);
    inputBox->setMaximumHeight(200);

    sendBtn = new QPushButton(basicInterface);
    sendBtn->setText("Send");

    QSpacerItem *btnSpacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addSpacerItem(btnSpacer);
    btnLayout->addWidget(sendBtn);

    QVBoxLayout *sendMessagesLayout = new QVBoxLayout;
    sendMessagesLayout->addWidget(inputBox);
    sendMessagesLayout->addLayout(btnLayout);

    QVBoxLayout *totalLayout = new QVBoxLayout(basicInterface);
    basicInterface->setLayout(totalLayout);
    totalLayout->addLayout(messageLayout);
    totalLayout->addLayout(sendMessagesLayout);
}

void ServerInterface::newClientConnected()
{
    QTcpSocket *newClient = serverConnection.tcpServer->nextPendingConnection();
    qDebug() << newClient->socketDescriptor();
    connectedClients.push_back(newClient);
    connect(newClient, &QTcpSocket::disconnected, this, &ServerInterface::clientDisconnected);
    connect(newClient, &QTcpSocket::readyRead, this, &ServerInterface::readMessages);
    qDebug() << "New client connected";
}

void ServerInterface::clientDisconnected()
{
    if(auto client = dynamic_cast<QTcpSocket *>(sender())) {
        connectedClients.removeAll(client);
    }
    qDebug() << "Client disconnected";
}


void ServerInterface::readMessages()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    previousSocketDescriptor = socket->socketDescriptor();
    qDebug() << previousSocketDescriptor;
    QString messagesFromClients = socket->readAll();
    qDebug() << "Got message from Client: " << messagesFromClients;
    QStringList usernameAndMessages = messagesFromClients.split(" %1");
    inputBox->clear();
    inputBox->setText(usernameAndMessages[1]);
    emit messagesRead();
}

void ServerInterface::sendMessages() {
    QByteArray messagesToClients;
    QDataStream out(&messagesToClients, QIODevice::WriteOnly);
    QString currentTime = QTime::currentTime().toString("h:mm:ss AP");
    out << inputBox->toPlainText() + "currentTime:" + currentTime;

    foreach (QTcpSocket *client, connectedClients) {
        if(client->socketDescriptor() != previousSocketDescriptor) {
            client->write(messagesToClients);
        }
    }
}

void ServerInterface::clearSocketDescriptor()
{
    previousSocketDescriptor = 0;
}
