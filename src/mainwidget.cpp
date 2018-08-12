#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QHostInfo>
#include "chatwindow.h"
#include <QSound>
#include <QTime>
#include <QDir>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    setWindowLayout();
    setSocket();
}

MainWidget::~MainWidget()
{
    //remove chat history when app is closed;
    QDir dir(ChatWindow::dataPath);
    dir.removeRecursively();
    delete ui;
}

void MainWidget::setWindowLayout()
{
    setWindowTitle("Chatty");

    ui->userLbl->setText(QHostInfo::localHostName());

    ui->statusBox->addItem(tr("Online"));
    ui->statusBox->addItem(tr("Offline"));

    connect(ui->statusBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWidget::statusChanged);
    connect(ui->userList, &QListWidget::itemDoubleClicked, this, &MainWidget::userListDoubleClicked);
}

void MainWidget::setSocket()
{
    socket = new Socket(this);

    connect(socket, &Socket::connected, this, &MainWidget::connected);
    connect(socket, &Socket::unconnected, this, &MainWidget::unconnected);
    connect(socket, &Socket::readyRead, this, &MainWidget::readMessage);

    socket->connectToServer();
}

void MainWidget::addToUserVec(QString username)
{
    QListWidgetItem *item = new QListWidgetItem(ui->userList);

    item->setText(username);

    userVec.push_back(item);

    ui->userList->addItem(item);
}

void MainWidget::connected()
{
    ui->statusBox->setCurrentText("Online");

    socket->write("localHostName: " + QHostInfo::localHostName().toUtf8());
}

void MainWidget::unconnected()
{
    ui->statusBox->setCurrentText("Offline");
}

void MainWidget::statusChanged(int index)
{
    if(index == 0) {
        socket->connectToServer();
    }
    else {
        socket->close();
    }
}

void MainWidget::readMessage()
{
    QString message = socket->readAll();
    qDebug() << message;

    if(message.contains(" NextSocket: ")) {
        QStringList onlineUsers = message.split(" NextSocket: ");
        onlineUsers.pop_back();
        for(int i = 0; i < onlineUsers.size(); ++i) {
            if(std::find(userIDVec.begin(), userIDVec.end(), onlineUsers[i].split(" ")[1]) != userIDVec.end()) {
                ;
            }
            else {
                addToUserVec(onlineUsers[i].split(" ")[0]);
                userIDVec.push_back(onlineUsers[i].split(" ")[1]);
            }
        }
    }
    else if(message.contains("disconnectedSocket: ")) {
        QString socketID = message.split("disconnectedSocket: ")[1];
        int pos = std::find(userIDVec.begin(), userIDVec.end(), socketID) - userIDVec.begin();
        userIDVec.erase(userIDVec.begin() + pos);
        delete userVec[pos];
        userVec.erase(userVec.begin() + pos);
    }
    else if(message.contains("messageFrom: ")) {
        QStringList stringList = message.split("messageFrom: ");
        qDebug() << stringList[1].split(" hereAreMessages: ")[0] << stringList[1].split(" hereAreMessages: ")[1];
        int pos = std::find(userIDVec.begin(), userIDVec.end(), stringList[1].split(" hereAreMessages: ")[0]) - userIDVec.begin();
        userVec[pos]->setTextColor(Qt::red);

        QSound::play(":/Sounds/notification.wav");

        QString currentTime = QTime::currentTime().toString("h:mm:ss AP");
        QString msColor = "<font color = \"green\">";

        ChatWindow::saveChatHistory(userIDVec[pos], msColor + currentTime + "<br>", msColor + stringList[1].split(" hereAreMessages: ")[1] + "<br>");
    }
}

void MainWidget::userListDoubleClicked(QListWidgetItem *item)
{
    ChatWindow *chatWindow = new ChatWindow(0, item->text(), userIDVec[ui->userList->currentRow()].toInt());
    chatWindow->show();

    connect(chatWindow, &ChatWindow::messageToWrite, [this](QString message){ socket->write(message.toUtf8()); });
}
