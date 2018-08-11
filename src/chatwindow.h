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
    ChatWindow(QWidget *parent = 0, QString name = "");
    ~ChatWindow();

private:
    Ui::ChatWindow *ui;

protected:
    void closeEvent(QCloseEvent *);
};

#endif // CHATWINDOW_H
