#include <QApplication>
#include "mainwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWidget userList;
    userList.show();

    return a.exec();
}
