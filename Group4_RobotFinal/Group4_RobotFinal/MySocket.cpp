#include "MySocket.h"

using namespace std;

MySocket::MySocket(SocketType socketType, string ip, unsigned int port, ConnectionType connType, unsigned int bufferSize) {
	if (bufferSize <= 0) {
		bufferSize = DEFAULT_SIZE;
	}

	WelcomeSocket = INVALID_SOCKET;
	ConnectionSocket = INVALID_SOCKET;

	SvrAddr.sin_family = AF_INET;
	SetIPAddr(ip);
	setPort(port);
	int addrLen = sizeof(SvrAddr);
	connectionType = connType;
	Buffer = nullptr;

	int sock = connType == TCP ? SOCK_STREAM : SOCK_DGRAM;
	int protocol = connType == TCP ? IPPROTO_TCP : IPPROTO_UDP;

	if (socketType == SERVER) {
		WelcomeSocket = socket(AF_INET, sock, protocol);
		if (WelcomeSocket == INVALID_SOCKET) {
			cout << "Unable to create WelcomeSocket" << endl;
		}

		int binded = bind(WelcomeSocket, (struct sockaddr*)&SvrAddr, addrLen);
		if (binded == -1) {
			cout << "Unable to bind welcome socket" << endl;
		}

		if (protocol == IPPROTO_TCP) {
			int listened = listen(WelcomeSocket, 1);
			if (listened == -1) {
				cout << "Unable to listen for connections" << endl;
			}

			ConnectionSocket = accept(WelcomeSocket, (struct sockaddr*)&SvrAddr, &addrLen);
		}
	}
	else if (socketType == CLIENT) {
		ConnectionSocket = socket(AF_INET, sock, protocol);
		if (ConnectionSocket == INVALID_SOCKET) {

		}
	}

	
	Buffer = new char[bufferSize];
}
MySocket::~MySocket() {
	if (Buffer != nullptr) {
		delete[] Buffer;
	}
}
void MySocket::ConnectTCP() {
	if (connectionType == UDP || WelcomeSocket != INVALID_SOCKET) {
		return;
	}

	

}
void MySocket::DisconnectTCP() {
	closesocket(ConnectionSocket);

}
void MySocket::SendData(const char* data, int size) {


}
int MySocket::GetData(char* src) {


}
string MySocket::GetIPAddr() {
	return SvrAddr.sin_addr.s_addr
}
void MySocket::SetIPAddr(string ip) {
	if (WelcomeSocket == INVALID_SOCKET || ConnectionSocket == INVALID_SOCKET)
		SvrAddr.sin_addr.s_addr = inet_addr(ip.c_str());
	else 
		cout << "cannot change ip address socket is already open or connected" << endl;
}
void MySocket::setPort(int port) {
	if (WelcomeSocket == INVALID_SOCKET || ConnectionSocket == INVALID_SOCKET)
		SvrAddr.sin_port = htons(port);
	else 
		cout << "cannot change port socket is already open or connected" << endl;
}
int MySocket::GetPort() {
	return Port;
}
SocketType MySocket::GetType() {
	return mySocket;
}
void MySocket::SetType(SocketType type) {
	if (WelcomeSocket == INVALID_SOCKET || ConnectionSocket == INVALID_SOCKET) {
		if (type == CLIENT)
			mySocket = CLIENT;
		else if (type == SERVER)
			mySocket = SERVER;
	}
	else
		cout << "cannot change type socket is already open or connected" << endl;
}