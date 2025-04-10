#include "MySocket.h"

using namespace std;

MySocket::MySocket(SocketType socketType, string ip, unsigned int port, ConnectionType connType, unsigned int bufferSize) {
	if (bufferSize <= 0) {
		bufferSize = DEFAULT_SIZE;
	}

	WelcomeSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (WelcomeSocket == INVALID_SOCKET) {
		cout << "Unable to create WelcomeSocket" << endl;
	}

	if (connType == UDP) {
		ConnectionSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	} else if (connType == TCP) {
		ConnectionSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}

	if (ConnectionSocket == INVALID_SOCKET) {
		cout << "Unable to create ConnectionSocket" << endl;
	}

	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());
	serverAddress.sin_port = htons(port);

	Buffer = nullptr;
	Buffer = new char[bufferSize];
}
MySocket::~MySocket() {
	if (Buffer != nullptr) {
		delete[] Buffer;
	}
}
void MySocket::ConnectTCP() {


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
	
}
void MySocket::setPort(int port) {
	if (WelcomeSocket == ) {

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