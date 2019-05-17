#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
	int serverSocket;
	struct sockaddr_in serverAddr;
	
	if (argc != 2) {
		cout << "Arugment issue." << endl;
		exit(1);
	}

	serverSocket = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(atoi(argv[1]));

	if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
		cout << "Bind error." << endl;
	}					
			
	if (listen(serverSocket, 5) == -1) {
		cout << "Listen error." << endl;
	}	

	int clientSocket;
	socklen_t clientAddrSize = sizeof(clientSocket);
	struct sockaddr_in clientAddr;

	while (true) {
		clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);
		if (clientSocket == -1) {
			cout << "Accept error." << endl;
		}
		else {
			cout << "New client connected." << endl;
		}

		close(clientSocket);
	}

	close(serverSocket);

	return 0;
}
