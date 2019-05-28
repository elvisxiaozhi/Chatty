#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include "customtextedit.h"

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
    QString userID;
    CustomTextEdit *inputEdit;

    const QString SEND_USER_INFO = "01";

    void createInputEdit();
    QString generateID();
    void createSocket();

private slots:
    void on_sendButton_clicked();
    void sendUserInfo();
};

#endif // WIDGET_H
