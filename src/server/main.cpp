#include <QCoreApplication>
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Make a server and starts it
    Server server;
    server.startServer();

    return a.exec();
}
