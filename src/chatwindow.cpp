#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QCloseEvent>
#include <QDebug>
#include <QTime>

ChatWindow::ChatWindow(QWidget *parent, QString name, int ID) :
    QWidget(parent),
    ui(new Ui::ChatWindow), socketID(ID)
{
    ui->setupUi(this);
    ui->recieverLbl->setText(name);

    connect(ui->sendBtn, &QPushButton::clicked, this, &ChatWindow::sendBtnClicked);
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

void ChatWindow::sendBtnClicked()
{
    QString message;
    message.append("messageSendTo: " + QString::number(socketID).toUtf8() + " hereAreMessages: " + ui->inputBox->toPlainText().toUtf8());
    emit messageToWrite(message);

    QString currentTime = QTime::currentTime().toString("h:mm:ss AP");
    QString msColor = "<font color = \"blue\">";
    ui->msBox->insertHtml(msColor + currentTime + "<br>");
    ui->msBox->insertHtml(msColor + ui->inputBox->toPlainText() + "<br>");

    ui->inputBox->clear();
}
