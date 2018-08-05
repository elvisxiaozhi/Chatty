#include "server.h"
#include "socketthread.h"

Server::Server(QObject *parent) :
    QTcpServer(parent)
{
}

void Server::startServer()
{
    if(!listen(QHostAddress::Any, 6666)) {
        qDebug() << "Could not start server";
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    SocketThread *thread = new SocketThread(socketDescriptor, this);

    threadVec.push_back(thread);

    // once a thread is not needed, it will be beleted later
    connect(thread, &SocketThread::finished, [&, socketDescriptor]() {
        for(int i = 0; i < threadVec.size(); ++i) {
            if(threadVec[i]->socketDescriptor == socketDescriptor) {
                threadVec[i]->deleteLater();
                threadVec.erase(threadVec.begin() + i);
            }
        }
    });

    thread->start();
}
