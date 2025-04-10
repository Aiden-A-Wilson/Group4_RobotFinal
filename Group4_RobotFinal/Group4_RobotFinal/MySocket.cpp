#include "MySocket.h"

using namespace std;

MySocket::MySocket(SocketType socketType, string ip, unsigned int port, ConnectionType connType, unsigned int bufferSize) {
	if (bufferSize <= 0) {
		bufferSize = DEFAULT_SIZE;
	}

	SvrAddr.sin_family = AF_INET;
	SetIPAddr(ip);
	setPort(port);
	int addrLen = sizeof(SvrAddr);
	connectionType = connType;

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
		}
	}
	else if (socketType == CLIENT) {
		if (connType == TCP) {

		}
		else if (connType == UDP) {

		}
	}

	Buffer = nullptr;
	Buffer = new char[bufferSize];
}
MySocket::~MySocket() {
	if (Buffer != nullptr) {
		delete[] Buffer;
	}
}
void MySocket::ConnectTCP() {
	if (connectionType == UDP) {
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


}
void MySocket::SetIPAddr(string ip) {
	SvrAddr.sin_addr.s_addr = inet_addr(ip.c_str());
}
void MySocket::setPort(int port) {
	SvrAddr.sin_port = htons(port);
	if (WelcomeSocket == INVALID_SOCKET) {

	}
	else {

	}
	
}
int MySocket::GetPort() {
	return Port;
}
SocketType MySocket::GetType() {
	return mySocket;
}
void MySocket::SetType(SocketType type) {

	if (type == CLIENT)
		mySocket = CLIENT;
	else if (type == SERVER)
		mySocket = SERVER;
}