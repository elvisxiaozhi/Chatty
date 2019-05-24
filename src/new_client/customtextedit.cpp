#include "customtextedit.h"
#include <QKeyEvent>

CustomTextEdit::CustomTextEdit(QWidget *parent) : QTextEdit(parent)
{

}

void CustomTextEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control + Qt::Key_Return) {
        QTextEdit::keyPressEvent(event);
    }
    else if (event->key() == Qt::Key_Return) {
        emit returnPressed();
    }
    else {
        QTextEdit::keyPressEvent(event);
    }
}
