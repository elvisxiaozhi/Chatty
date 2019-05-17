#include "widget.h"
#include "ui_widget.h"
#include <QHostAddress>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress("34.80.123.141"), 6666);

    if (!socket->waitForConnected()) {
        qDebug() << "Failed to connect";
    }
    else {
        qDebug() << "Connected to server";
    }
}

Widget::~Widget()
{
    delete ui;
}
