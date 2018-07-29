#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowLayout();
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
