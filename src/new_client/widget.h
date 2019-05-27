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

    void createInputEdit();
    void generateID();

private slots:
    void on_sendButton_clicked();
};

#endif // WIDGET_H
