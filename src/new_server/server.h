#ifndef SERVER_H
#define SERVER_H

#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
#include <map>
#include <cstring>

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
    static map<string, int> clientSocketInfo;
    static map<string, string> userInfo;
    static pthread_mutex_t mutex;
    static int serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrSize;

    static const int HEADER_SIZE = 2;
    static const int USER_ID_SIZE = 10;
    static const int USERNAME_SIZE = 20;
//    static const string GET_USER_INFO = "01";
    static const string SEND_USERS_INFO;

    static void *clientHandler(void *);
    static string bindUserIDToSocket(const int, char *);
    static void setUserInfo(const string, char *);
    static void sendUserInfoToClients();
    static string decipherMessage(char *);
    static void messageType(const string);
    static void sendMsg(char *, int);
};

#endif // SERVER_H
