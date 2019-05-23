#include "widget.h"
#include "ui_widget.h"
#include <QHostAddress>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);

//    socket->connectToHost(QHostAddress("34.80.123.141"), 6666);
    socket->connectToHost(QHostAddress::LocalHost, 6666);

    if (!socket->waitForConnected()) {
        qDebug() << "Failed to connect";
    }
    else {
        qDebug() << "Connected to server";
    }

    connect(socket, &QTcpSocket::readyRead, [this](){ qDebug() << socket->readAll(); });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_sendButton_clicked()
{
    QString msg = ui->lineEdit->text() + "\n";
    socket->write(msg.toUtf8());
}
