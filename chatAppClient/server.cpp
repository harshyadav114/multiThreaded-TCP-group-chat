#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<tchar.h>
#include<thread>
#pragma comment(lib,"ws2_32.lib")
int PORT = 12345;
#include<vector>
using namespace std;

bool Initialize() {
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

void InteractWithClient(SOCKET clientSocket, vector<SOCKET>& clients) {

	cout << endl << " client connected";

	char buff[4096];

	while (1) {
		int bytesecieved = recv(clientSocket, buff, sizeof(buff), 0);

		if (bytesecieved <= 0) {
			cout << endl << "client disconnectd";
			break;
		}

		string messageClient(buff, bytesecieved);
		cout << endl << "message from client :" << messageClient;

		for (auto client : clients) {
			if (client != clientSocket) {
				send(client, messageClient.c_str(), messageClient.length(), 0);
			}
		}
	}

	auto it = find(clients.begin(), clients.end(), clientSocket);
	if (it != clients.end()) {
		clients.erase(it);
	}
	closesocket(clientSocket);

}


int main() {

	Initialize();

	if (!Initialize()) {
		cout << endl << "Initializiton failed";
		WSACleanup();
		exit(EXIT_FAILURE);
	}

	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (listenSocket == INVALID_SOCKET) {
		cout << endl << "socket creation is failed";
		WSACleanup();
		exit(EXIT_FAILURE);
	}

	//create adders struct

	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(PORT);

	if (InetPton(AF_INET, _T("0.0.0.0"), &serveraddr.sin_addr) != 1) {
		cout << endl << "setting adders struct fialed";
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	//binding

	if (bind(listenSocket, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr))) {
		cout << endl << " bind failed";
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		cout << endl << " listening to socket fails";
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	cout << endl << "server start listening on port:" << PORT;
	vector<SOCKET>clients;
	while (1) {
		//accept
		SOCKET clientSocket = accept(listenSocket, NULL, NULL);

		if (clientSocket == INVALID_SOCKET) {
			cout << endl << "invalid socket";
		}
		clients.push_back(clientSocket);
		thread t1(InteractWithClient, clientSocket, std::ref(clients));
		t1.detach();
	}


	closesocket(listenSocket);

	WSACleanup();
	return 0;
}