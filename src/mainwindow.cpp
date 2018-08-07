#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStatusBar>
#include <QLabel>
#include <QPushButton>
#include <QTime>
#include <QHostInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowLayout();

    connection = new Connection(this);

    connect(connection, &Connection::connected, this, &MainWindow::connected);
    connect(connection, &Connection::unconnected, this, &MainWindow::unconnected);
    connect(connection, &Connection::readyRead, this, &MainWindow::readMessage);
    connect(ui->sendBtn, &QPushButton::clicked, this, &MainWindow::sendMessage);

    connection->connectToServer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setWindowLayout()
{
    setWindowTitle("Chatty");

    localHostName = QHostInfo::localHostName();

    ui->msBox->setReadOnly(true);
    ui->msBox->setStyleSheet("border: 1px solid black");

    ui->userList->setStyleSheet("border: 1px solid black");
    ui->userList->addItem(localHostName);

    setOfflineStatusBar();
    statusBar()->addWidget(statusWidget);
}

void MainWindow::setOfflineStatusBar()
{
    statusWidget = new QWidget(this);

    QHBoxLayout *hLayout = new QHBoxLayout(statusWidget);

    QLabel *label = new QLabel(statusWidget);
    label->setText(tr("Offline"));

    QPushButton *btn = new QPushButton(statusWidget);
    btn->setText(tr("Reconnect"));

    hLayout->addWidget(label);
    hLayout->addStretch();
    hLayout->addWidget(btn);
    statusWidget->setLayout(hLayout);

    connect(btn, &QPushButton::clicked, [this](){ connection->connectToServer(); });
}

void MainWindow::connected()
{
    statusBar()->removeWidget(statusWidget);
    statusBar()->showMessage(tr("Online"));

    connection->write(localHostName.toUtf8());
}

void MainWindow::unconnected()
{
    statusBar()->clearMessage();
    statusBar()->removeWidget(statusWidget);
    statusBar()->addWidget(statusWidget);
    statusWidget->show();
}

void MainWindow::sendMessage()
{
    QString message = ui->inputBox->toPlainText();
    connection->write(message.toUtf8());

    QString currentTime = QTime::currentTime().toString("h:mm:ss AP");
    QString msColor = "<font color = \"blue\">";
    ui->msBox->insertHtml(msColor + currentTime + "<br>");
    ui->msBox->insertHtml(msColor + ui->inputBox->toPlainText() + "<br>");

    ui->inputBox->clear();
}

void MainWindow::readMessage()
{
    QString message = connection->readAll();
    qDebug() << message;
}
