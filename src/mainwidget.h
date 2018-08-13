#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "socket.h"

namespace Ui {
class MainWidget;
}

class QListWidgetItem;
class ChatWindow;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private:
    Ui::MainWidget *ui;

    Socket *socket;
    QVector<QListWidgetItem *> userVec;
    QVector<QString> userIDVec;
    QVector<ChatWindow *> chatWindowVec;

    void setWindowLayout();
    void setSocket();
    void addToUserVec(QString);
    void recieveSocket(QString);
    void socketDisconnected(QString);
    void recieveMessage(QString);
    void createChatWindow(QListWidgetItem *);
    bool hasChatWindow(int);
    int returnChatWindowIndex(int);

private slots:
    void connected();
    void unconnected();
    void statusChanged(int);
    void readMessage();
    void userListDoubleClicked(QListWidgetItem *);
};

#endif // MAINWIDGET_H
