#include "mainwindow.h"
#include <QApplication>
#include "mainwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

    MainWidget userList;
    userList.show();

    return a.exec();
}
