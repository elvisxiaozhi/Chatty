#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "socket.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private:
    Ui::MainWidget *ui;

    Socket *socket;

    void setWindowLayout();
    void setSocket();

private slots:
    void connected();
    void unconnected();
    void statusChanged(int);
};

#endif // MAINWIDGET_H
