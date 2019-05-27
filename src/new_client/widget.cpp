#include "widget.h"
#include "ui_widget.h"
#include <QHostAddress>
#include <QShortcut>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    generateID();
    ui->textEdit->setText(userID);

    socket = new QTcpSocket(this);

//    socket->connectToHost(QHostAddress("34.80.123.141"), 6666);
    socket->connectToHost(QHostAddress("192.168.56.101"), 6666);

    if (!socket->waitForConnected()) {
        qDebug() << "Failed to connect";
    }
    else {
        qDebug() << "Connected to server";
    }
    generateID();

    createInputEdit();

    connect(socket, &QTcpSocket::readyRead, [this](){ qDebug() << socket->readAll();
        qDebug() << socket->socketDescriptor(); });
}

Widget::~Widget()
{
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

void Widget::generateID()
{
    QString id = QString::number(rand() % UINT_MAX);
    int i, n = QString::number(UINT_MAX).size() - id.size();
    for (i = 0; i < n; ++i) {
        id.push_front("0");
    }

    userID = id;
}

void Widget::on_sendButton_clicked()
{
    QString msg = inputEdit->toPlainText() + "\n";
    socket->write(msg.toUtf8());
    inputEdit->clear();
}
