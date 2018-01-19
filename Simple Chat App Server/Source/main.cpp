#include "serverinterface.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServerInterface w;
    w.show();

    return a.exec();
}
