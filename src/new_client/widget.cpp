#include "widget.h"
#include "ui_widget.h"
#include <QHostAddress>
#include <QShortcut>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    userID = generateID();
    createInputEdit();
    createSocket();

    connect(ui->friendsList, &QListWidget::itemDoubleClicked, this, &Widget::friendsListDoubleClicked);
}

Widget::~Widget()
{
    socket->close();
    delete ui;
}

void Widget::createInputEdit()
{
    inputEdit = new CustomTextEdit(this);
    ui->verticalLayout->insertWidget(1, inputEdit);

    connect(inputEdit, &CustomTextEdit::returnPressed, this, &Widget::on_sendButton_clicked);

    QShortcut *btnShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Return), this);
    connect(btnShortcut, &QShortcut::activated, [this](){ qDebug() << "Key"; });
}

QString Widget::generateID()
{
    QString id = QString::number(rand() % UINT_MAX);
    int i, n = QString::number(UINT_MAX).size() - id.size();
    for (i = 0; i < n; ++i) {
        id.push_front("0");
    }

    return id;
}

void Widget::createSocket()
{
    socket = new QTcpSocket(this);

//    socket->connectToHost(QHostAddress("34.80.123.141"), 6666);
    socket->connectToHost(QHostAddress("192.168.56.101"), 6666);
    if (!socket->waitForConnected()) {
        qDebug() << "Failed to connect";
    }
    else {
        sendUserInfo(); //change this later
    }

    connect(socket, &QTcpSocket::readyRead, this, &Widget::decipherMessage);
}

void Widget::setFriendsList(QString msg)
{
    QStringList friendsList;
    int i, n = msg.size() / (USER_ID_SIZE + USERNAME_SIZE);
    QString id, name;
    for (i = 0; i < n; ++i) {
        id = msg.left(USER_ID_SIZE);
        msg.remove(0, USER_ID_SIZE);
        name = msg.left(USERNAME_SIZE);
        msg.remove(0, USERNAME_SIZE);

        friendsInfo.insert(id, name);
        friendsList.push_back(name);
    }

    updateFriendsList(friendsList);
}

void Widget::on_sendButton_clicked()
{
    QString msg = inputEdit->toPlainText() + "\n";
    socket->write(msg.toUtf8());
    inputEdit->clear();
}

int Widget::sendUserInfo()
{
    QString msg = SEND_USER_INFO + userID + username;
    int i, n = USERNAME_SIZE - username.size();
    for (i = 0; i < n; ++i) {
        msg.push_back("*");
    }

    return socket->write(msg.toUtf8());
}

void Widget::updateFriendsList(const QStringList friendsList)
{
    int i, n = friendsList.size();
    for (i = 0; i < n; ++i) {
        QListWidgetItem *item = new QListWidgetItem(ui->friendsList);
        item->setText(QString(friendsList[i]).remove("*"));

        ui->friendsList->addItem(item);
    }
}

void Widget::decipherMessage()
{
    QString msg = socket->readAll();
    qDebug() << msg;
    QString header = msg.left(HEADER_SIZE);
    msg = msg.remove(0, HEADER_SIZE);

    if (header == UPDATE_FRIENDS_LIST) {
        setFriendsList(msg);
    }
}

void Widget::friendsListDoubleClicked()
{
    qDebug() << "Clicked";
}
