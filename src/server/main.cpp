#include <QCoreApplication>
#include "connection.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Connection connection;

    return a.exec();
}
