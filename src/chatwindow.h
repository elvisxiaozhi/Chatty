#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    ChatWindow(QWidget *parent = 0, QString name = "", int ID = 0);
    ~ChatWindow();
    int socketID;
    static QString dataPath;

    static void checkDataFilePath();
    static void saveChatHistory(QString, QString, QString);
    void recieveMessage(QString, QString);

private:
    Ui::ChatWindow *ui;

protected:
    void closeEvent(QCloseEvent *);

signals:
    void messageToWrite(QString);
    void closingWindow(int);

private slots:
    void sendBtnClicked();
};

#endif // CHATWINDOW_H
