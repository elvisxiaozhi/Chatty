#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include "customtextedit.h"
#include <QMap>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    QTcpSocket *socket;
    QString userID, username = "Theodore";
    QMap<QString, QString> friendsInfo;
    CustomTextEdit *inputEdit;

    const int HEADER_SIZE = 2;
    const int USER_ID_SIZE = 10;
    const int USERNAME_SIZE = 20;
    const QString SEND_USER_INFO = "01";
    const QString UPDATE_FRIENDS_LIST = "02";

    void createInputEdit();
    QString generateID();
    void createSocket();
    void setFriendsList(QString);

private slots:
    void on_sendButton_clicked();
    int sendUserInfo();
    void decipherMessage();
    void updateFriendsList(const QStringList);
    void friendsListDoubleClicked();
};

#endif // WIDGET_H
