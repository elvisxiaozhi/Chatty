#ifndef SERVER_H
#define SERVER_H

#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>

using std::vector;

class Server
{
public:
    Server();

private:
    static const int BUFF_SIZE = 100;
    static int *socksClient;
    static vector<int> clients;
    static pthread_mutex_t mutex;
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrSize;

    static void *clientHandler(void *arg);
    static void sendMsg(char *msg, int len);
};

#endif // SERVER_H
