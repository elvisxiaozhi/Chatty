#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowLayout();

    connection = new Connection(this);

    connect(connection, &Connection::connected, this, &MainWindow::connected);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::sendMessage);

    connection->connect();
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

    statusBar()->showMessage(tr("Offline"));
}

void MainWindow::connected()
{
    statusBar()->showMessage(tr("Online"));
}

void MainWindow::sendMessage()
{
    QString message = ui->inputBox->toPlainText();
    connection->sendMessage(message);
}
