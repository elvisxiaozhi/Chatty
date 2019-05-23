#ifndef SERVER_H
#define SERVER_H

#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>

class Server
{
public:
    Server();
    ~Server();

private:
    static const int BUFF_SIZE = 100;
    const int MAX_CLIENT = 255;
    static int countClient;
    static int *socksClient;
    static pthread_mutex_t mutex;
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrSize;

    static void *clientHandler(void *arg);
    static void sendMsg(char *msg, int len);
};

#endif // SERVER_H
