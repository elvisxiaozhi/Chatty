#include "server.h"
#include <iostream>
#include <cstring>
#include <unistd.h>

using std::cout;
using std::endl;

vector<int> Server::clients;
pthread_mutex_t Server::mutex;

Server::Server()
{
    cout << "What's the port: ";
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
        clients.push_back(clientSocket);
        pthread_mutex_unlock(&mutex);

        pthread_create(&pthreadID, nullptr, &Server::clientHandler, (void *)&clientSocket);
        pthread_detach(pthreadID);//销毁线程

        cout << "Client IP: " << inet_ntoa(clientAddr.sin_addr) << endl;
    }

    close(serverSocket);
    pthread_mutex_destroy(&mutex);
}

void *Server::clientHandler(void *arg)
{
    int clientSocket = *((int *)arg);
    int strLen = 0;
    char msg[BUFF_SIZE];

    while ((strLen = read(clientSocket, msg, sizeof(msg))) != 0) {
        sendMsg(msg, strLen);
    }

    //when read == 0, client disconnected;
    //remove disconnencted client
    pthread_mutex_lock(&mutex);
    int i, n = clients.size();
    for (i = 0; i < n; ++i){
        if (clientSocket == clients[i]) {
            clients.erase(clients.begin() + i);
            cout << "Client disconnected" << endl;
        }
        break;
    }
    pthread_mutex_unlock(&mutex);
    close(clientSocket);

    return nullptr;
}

void Server::sendMsg(char *msg, int len)
{
    pthread_mutex_lock(&mutex);
    int i, n = clients.size();
    for (i = 0; i < n; ++i){
        write(clients[i], msg, len);
    }
    pthread_mutex_unlock(&mutex);
}
