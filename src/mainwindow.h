#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "connection.h"
#include <QModelIndex>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Connection *connection;
    QWidget *statusWidget;
    QString localHostName;

    void setWindowLayout();
    void setOfflineStatusBar();

private slots:
    void connected();
    void unconnected();
    void sendMessage();
    void readMessage();
    void usernameSelected(QModelIndex);
};

#endif // MAINWINDOW_H
