#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<tchar.h>
#pragma comment(lib,"ws2_32.lib")
int PORT = 12345;
using namespace std;

bool Initialize() {
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}


int main() {

	Initialize();

	if (!Initialize()) {
		cout << endl << "Initializiton failed";
		WSACleanup();
		exit(EXIT_FAILURE);
	}

	SOCKET s;
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET) {
		cout << endl << "socket is invalid ";
		return 1;

	}
	string serveradderes = "127.0.0.1";
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, serveradderes.c_str(), &serveraddr.sin_addr) != 1) {
		cout << endl << "setting adders struct fialed";
		closesocket(s);
		WSACleanup();
		return 1;
	}

	if (connect(s, reinterpret_cast<sockaddr*> (&serveraddr), sizeof(serveraddr))) {
		cout << endl << " not able to connect to server";
		closesocket(s);
		WSACleanup();
		return 1;
	}
	cout << endl << "sucessfulyy coonect to server";

	string message = "hell0 mf";
	int bytesend= send(s, message.c_str(), message.length(), 0);

	if (bytesend == SOCKET_ERROR) {
		cout << endl << " send failed";
	}
	



	closesocket(s);

	WSACleanup();
	return 0;

}