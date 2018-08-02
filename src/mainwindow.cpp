#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStatusBar>
#include <QLabel>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowLayout();

    connection = new Connection(this);

    connect(connection, &Connection::connected, this, &MainWindow::connected);
    connect(connection, &Connection::unconnected, this, &MainWindow::unconnected);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::sendMessage);

    connection->connectToServer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setWindowLayout()
{
    setWindowTitle("Chatty");

    ui->msBox->setReadOnly(true);
    ui->msBox->setStyleSheet("border: 1px solid black");

    ui->userList->setStyleSheet("border: 1px solid black");

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
}
