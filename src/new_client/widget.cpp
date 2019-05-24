#include "widget.h"
#include "ui_widget.h"
#include <QHostAddress>
#include <QShortcut>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);

    socket->connectToHost(QHostAddress("34.80.123.141"), 6666);
//    socket->connectToHost(QHostAddress::LocalHost, 6666);

    if (!socket->waitForConnected()) {
        qDebug() << "Failed to connect";
    }
    else {
        qDebug() << "Connected to server";
    }

    createInputEdit();

    connect(socket, &QTcpSocket::readyRead, [this](){ qDebug() << socket->readAll(); });
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
    connect(btnShortcut, &QShortcut::activated, [this](){ /*inputEdit->rep; */qDebug() << "Key"; });
}

void Widget::on_sendButton_clicked()
{
    QString msg = inputEdit->toPlainText() + "\n";
    socket->write(msg.toUtf8());
    inputEdit->clear();
}
