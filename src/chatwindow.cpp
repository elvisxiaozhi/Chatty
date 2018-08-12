#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QCloseEvent>
#include <QDebug>
#include <QTime>
#include <QDir>
#include <QStandardPaths>

QString ChatWindow::dataPath = "";

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

void ChatWindow::checkDataFilePath()
{
    QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    dataPath = homePath.first() + "/AppData/Local/Chatty";
    if(!QDir().exists(dataPath)) {
        QDir().mkdir(dataPath);
    }
}

void ChatWindow::saveChatHistory(QString fileName, QString time, QString history)
{
    checkDataFilePath();

    QString chatFile = dataPath + QString("/%1.txt").arg(fileName);
    QFile file(chatFile);
    if (file.open(QFile::Append | QFile::Text)) {
        QTextStream stream(&file);
        stream << time << endl;
        stream << history << endl;
        file.close();
    }
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

    saveChatHistory(QString::number(socketID), msColor + currentTime + "<br>", msColor + ui->inputBox->toPlainText() + "<br>");

    ui->inputBox->clear();
}
