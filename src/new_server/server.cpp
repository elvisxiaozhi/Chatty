#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

using std::cout;
using std::endl;

#define BUFF_SIZE 100
#define CLIENT_MAX 256

void *clientHandler(void *arg);
void sendMsg(char *msg, int len);

int countClient = 0;
int socksClient[CLIENT_MAX];
pthread_mutex_t mutex;

int main()
{
    int serverSocket, clientSocket;

    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrSize;

    cout << "What's the port: " << endl;
    std::string port;
    std::getline(std::cin, port);

    pthread_mutex_init(&mutex, NULL);
    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(atoi(port.c_str()));

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        cout << "Bind error." << endl;
    }

    if (listen(serverSocket, 5) == -1) {
        cout << "Listen error." << endl;
    }

    pthread_t pthreadID;
    while (true) {
        clientAddrSize = sizeof(clientAddr);
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);

        pthread_mutex_lock(&mutex);
        socksClient[countClient++] = clientSocket;//Lock 访问公共代码区
        pthread_mutex_unlock(&mutex);

        pthread_create(&pthreadID, NULL, clientHandler, (void *)&clientSocket);
        pthread_detach(pthreadID);//销毁线程

        cout << "Client IP: " << inet_ntoa(clientAddr.sin_addr) << endl;
    }

    close(serverSocket);
    pthread_mutex_destroy(&mutex);

    return 0;
}

void *clientHandler(void *arg)
{
    int clientSocket = *((int *)arg);
    int strLen = 0, i;
    char msg[BUFF_SIZE];

    while ((strLen = read(clientSocket, msg, sizeof(msg))) != 0) {
        sendMsg(msg, strLen);
    }

    pthread_mutex_lock(&mutex);
    for (i = 0; i < countClient; ++i){//remove disconnencted client
        if (clientSocket == socksClient[i]) {
            while (i++ <= countClient - 1) {
                socksClient[i] = socksClient[i + 1];
            }
            break;
        }
    }
    countClient--;
    pthread_mutex_unlock(&mutex);
    close(clientSocket);

    return nullptr;
}
void sendMsg(char *msg, int len)//send message to all clients
{
    int i;
    pthread_mutex_lock(&mutex);
    for (i = 0; i < countClient; ++i){
        write(socksClient[i], msg, len);
    }
    pthread_mutex_unlock(&mutex);
}
