#include "server.h"
#include <iostream>
#include <unistd.h>

using std::cout;
using std::endl;

map<string, int> Server::clientSocketInfo;
map<string, string> Server::userInfo;
pthread_mutex_t Server::mutex;
int Server::serverSocket;
const string Server::SEND_USERS_INFO = "02";

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
    serverAddr.sin_port = htons(/*atoi(port.c_str())*/6666);

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
    char msg[BUFF_SIZE];
    read(socket, msg, sizeof(msg));

    string userID = bindUserIDToSocket(socket, msg); //userID is needed later when clientSocket disconnected from server
    setUserInfo(userID, msg);

    sendUserInfoToClients();
    memset(msg, 0, sizeof(msg));

    int strLen = 0;
    while ((strLen = read(socket, msg, sizeof(msg))) != 0) {
        sendMsg(msg, strLen);

        messageType(decipherMessage(msg));

        cout << msg << endl;
        memset(msg, 0, sizeof(msg));
    }

    //when read == 0, client disconnected; then remove disconnencted client
    pthread_mutex_lock(&mutex);
    map<string ,int>::iterator clntIt = clientSocketInfo.find(userID);
    clientSocketInfo.erase(clntIt);

    map<string, string>::iterator usrIt = userInfo.find(userID);
    userInfo.erase(usrIt);
    cout << "Client disconnected" << "\t" << clientSocketInfo.size() << endl;
    pthread_mutex_unlock(&mutex);

    close(socket);

    return nullptr;
}

string Server::bindUserIDToSocket(const int socket, char *msg)
{
    string userID;
    int i, n = HEADER_SIZE + USER_ID_SIZE;
    for (i = HEADER_SIZE; i < n; ++i) {
        userID.push_back(msg[i]);
    }

    pthread_mutex_lock(&mutex);
    clientSocketInfo.insert(std::make_pair(userID, socket));
    pthread_mutex_unlock(&mutex);

    for(auto e : clientSocketInfo)
    {
       cout << e.first << " " << e.second << endl;
    }

    return userID;
}

void Server::setUserInfo(const std::string id, char *msg)
{
    string username;
    int i, n = HEADER_SIZE + USER_ID_SIZE + USERNAME_SIZE;
    for (i = HEADER_SIZE + USER_ID_SIZE; i < n; ++i) {
        username.push_back(msg[i]);
    }

    pthread_mutex_lock(&mutex);
    userInfo.insert(std::make_pair(id, username));
    pthread_mutex_unlock(&mutex);

    for(auto e : userInfo)
    {
       cout << e.first << " " << e.second << endl;
    }
}

void Server::sendUserInfoToClients()
{
    string msgStr = SEND_USERS_INFO;
    for (auto e : userInfo) {
        msgStr.append(e.first + e.second); //don't know why push back is not working
    }

    int len = msgStr.length();
    char msg[len];
    strcpy(msg, msgStr.c_str());
    for (auto e : clientSocketInfo) {
        write(e.second, msg, len);
    }
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
    for (auto e : clientSocketInfo) {
        write(e.second, msg, len);
    }
    pthread_mutex_unlock(&mutex);
}
