#ifndef SERVER_H
#define SERVER_H

#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
#include <string>
#include <map>

using std::map;
using std::vector;
using std::string;

class Server
{
public:
    Server();

private:
    static const int BUFF_SIZE = 255;
    static int *socksClient;
    static map<string, int> clientInfo;
    static pthread_mutex_t mutex;
    static int serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrSize;

    static const int HEADER_SIZE = 2;
    static const int USER_INFO_SIZE = 10;
    const string GET_USER_INFO = "01";
    const string SEND_USERS_INFO = "02";

    static void *clientHandler(void *);
    static string bindUserInfoToSocket(const int);
    static string decipherMessage(char *);
    static void messageType(const string);
    static void sendMsg(char *, int);
};

#endif // SERVER_H
