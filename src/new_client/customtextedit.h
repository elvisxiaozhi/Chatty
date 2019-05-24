#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <QTextEdit>

class CustomTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit CustomTextEdit(QWidget *parent = nullptr);

private:
    void keyPressEvent(QKeyEvent *);

signals:
    void returnPressed();
};

#endif // CUSTOMTEXTEDIT_H
