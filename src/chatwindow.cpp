#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QCloseEvent>
#include <QDebug>

ChatWindow::ChatWindow(QWidget *parent, QString name) :
    QWidget(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    ui->recieverLbl->setText(name);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::closeEvent(QCloseEvent *event)
{
    if(event->type() == QEvent::Close) {
        deleteLater();
    }
}
