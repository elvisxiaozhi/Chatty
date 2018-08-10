#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QHostInfo>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    setWindowLayout();
    setSocket();

    addToUserVec();
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::setWindowLayout()
{
    setWindowTitle("Chatty");

    ui->userLbl->setText(QHostInfo::localHostName());

    ui->statusBox->addItem(tr("Online"));
    ui->statusBox->addItem(tr("Offline"));

    connect(ui->statusBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWidget::statusChanged);
}

void MainWidget::setSocket()
{
    socket = new Socket(this);

    connect(socket, &Socket::connected, this, &MainWidget::connected);
    connect(socket, &Socket::unconnected, this, &MainWidget::unconnected);
    connect(socket, &Socket::readyRead, this, &MainWidget::readMessage);

    socket->connectToServer();
}

void MainWidget::addToUserVec()
{
    QListWidgetItem *item = new QListWidgetItem(ui->userList);
    item->setBackgroundColor(Qt::red);
    item->setText("Nila");

    userVec.push_back(item);

    ui->userList->addItem(item);
}

void MainWidget::connected()
{
    ui->statusBox->setCurrentText("Online");
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
}
