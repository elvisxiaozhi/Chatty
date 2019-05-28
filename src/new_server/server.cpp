#include "server.h"
#include <iostream>
#include <unistd.h>
#include <cstring>

using std::cout;
using std::endl;

map<string, int> Server::clientInfo;
pthread_mutex_t Server::mutex;
int Server::serverSocket;

Server::Server()
{
//    cout << "What's the port: ";
//    string port;
//    std::getline(std::cin, port);

    pthread_mutex_init(&mutex, NULL);
    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(/*atoi(port.c_str())*/6665);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        cout << "Bind error." << endl;
    }

    if (listen(serverSocket, 5) == -1) {
        cout << "Listen error." << endl;
    }

    int clientSocket;
    pthread_t pthreadID;
    while (true) {
        clientAddrSize = sizeof(clientAddr);
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);

        pthread_create(&pthreadID, nullptr, &Server::clientHandler, (void *)&clientSocket);
        pthread_detach(pthreadID);//销毁线程

        cout << "Client IP: " << inet_ntoa(clientAddr.sin_addr) << "\tPort: " << ntohs(clientAddr.sin_port) << endl;
    }

    close(serverSocket);
    pthread_mutex_destroy(&mutex);
}

void *Server::clientHandler(void *arg)
{
    int socket = *((int *)arg);
    string userID = bindUserInfoToSocket(socket); //userID is needed later when clientSocket disconnected from server

    int strLen = 0;
    char msg[BUFF_SIZE];
    while ((strLen = read(socket, msg, sizeof(msg))) != 0) {
        sendMsg(msg, strLen);

        messageType(decipherMessage(msg));

        cout << msg << endl;
        memset(msg, 0, sizeof(msg));
    }

    //when read == 0, client disconnected; then remove disconnencted client
    pthread_mutex_lock(&mutex);
    map<string ,int>::iterator it;
    it = clientInfo.find(userID);
    clientInfo.erase(it);
    cout << "Client disconnected" << "\t" << clientInfo.size() << endl;
    pthread_mutex_unlock(&mutex);

    close(socket);

    return nullptr;
}

string Server::bindUserInfoToSocket(const int socket)
{
    char msg[BUFF_SIZE];
    read(socket, msg, sizeof(msg));

    string userID;
    int i, n = USER_INFO_SIZE + HEADER_SIZE;
    for (i = HEADER_SIZE; i < n; ++i) {
        userID.push_back(msg[i]);
    }
    pthread_mutex_lock(&mutex);
    clientInfo.insert(std::make_pair(userID, socket));
    pthread_mutex_unlock(&mutex);

    for(auto elem : clientInfo)
    {
       std::cout << elem.first << " " << elem.second << endl;
    }

    return userID;
}

string Server::decipherMessage(char *msg)
{
    string header;
    int i;
    for (i = 0; i < 2; ++i) {
        header.push_back(msg[i]);
    }

    return header;
}

void Server::messageType(const std::string)
{
//    switch (header) {
//    case GET_USER_INFO:

//        break;
//    default:
//        break;
//    }
}

void Server::sendMsg(char *msg, int len)
{
    pthread_mutex_lock(&mutex);
    for (auto e : clientInfo) {
        write(e.second, msg, len);
    }
    pthread_mutex_unlock(&mutex);
}
