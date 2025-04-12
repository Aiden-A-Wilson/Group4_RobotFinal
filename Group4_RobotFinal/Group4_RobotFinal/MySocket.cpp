#include "MySocket.h"

using namespace std;

MySocket::MySocket(SocketType socketType, string ip, unsigned int port, ConnectionType connType, unsigned int bufferSize) {
	MaxSize = bufferSize;
	if (MaxSize <= 0) {
		MaxSize = DEFAULT_SIZE;
	}

	WelcomeSocket = INVALID_SOCKET;
	ConnectionSocket = INVALID_SOCKET;
	connectionType = connType;
	mySocket = socketType;
	Buffer = nullptr;

	SvrAddr.sin_family = AF_INET;
	SetIPAddr(ip);
	setPort(port);
	int addrLen = sizeof(SvrAddr);

	int sock = connType == TCP ? SOCK_STREAM : SOCK_DGRAM;
	int protocol = connType == TCP ? IPPROTO_TCP : IPPROTO_UDP;

	if (mySocket == SERVER) {
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
	else if (mySocket == CLIENT) {
		ConnectionSocket = socket(AF_INET, sock, protocol);
		if (ConnectionSocket == INVALID_SOCKET) {
			cout << "Unable to create ConnectionSocket" << endl;
		}
	}

	
	Buffer = new char[bufferSize];
}
MySocket::~MySocket() {
	if (Buffer != nullptr) {
		delete[] Buffer;
	}

	closesocket(ConnectionSocket);
	closesocket(WelcomeSocket);
}
void MySocket::ConnectTCP() {
	if (connectionType == UDP || WelcomeSocket != INVALID_SOCKET) {
		return;
	}

	int addrLen = sizeof(SvrAddr);
	int connected = connect(ConnectionSocket, (struct sockaddr*)&SvrAddr, addrLen);
	if (connected == SOCKET_ERROR) {
		cout << "Unable to connect to TCP server with ConnectionSocket" << endl;
	}
}

void MySocket::DisconnectTCP() {
	closesocket(ConnectionSocket);

}
void MySocket::SendData(const char* data, int size) {
	if (connectionType == TCP) {
		send(ConnectionSocket, data, size, 0);
	}
	else if (connectionType == UDP) {
		int addrLen = sizeof(SvrAddr);
		sendto(ConnectionSocket, data, size, 0, (struct sockaddr*)&SvrAddr, addrLen);
	}
}
int MySocket::GetData(char* RxBuffer) {
	int bytesWritten = 0;
	if (connectionType == TCP) {
		bytesWritten = recv(ConnectionSocket, Buffer, MaxSize, 0);
	}
	else if (connectionType == UDP) {
		int addrLen = sizeof(SvrAddr);
		bytesWritten = recvfrom(ConnectionSocket, Buffer, MaxSize, 0, (struct sockaddr*)&SvrAddr, &addrLen);
	}

	memcpy(RxBuffer, Buffer, MaxSize);

	return bytesWritten;
}
string MySocket::GetIPAddr() {
	return to_string(SvrAddr.sin_addr.s_addr);
}
void MySocket::SetIPAddr(string ip) {
	if (CanSet())
		SvrAddr.sin_addr.s_addr = inet_addr(ip.c_str());
	else 
		cout << "cannot change ip address socket is already open or connected" << endl;
}
void MySocket::setPort(int port) {
	if (CanSet())
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
	if (CanSet()) {
		mySocket = type;
	}
	else
		cout << "cannot change type socket is already open or connected" << endl;
}

bool MySocket::CanSet()
{
	if (mySocket == SERVER) {
		return WelcomeSocket == INVALID_SOCKET && ConnectionSocket == INVALID_SOCKET;
	}
	else if (mySocket == CLIENT) {
		return ConnectionSocket == INVALID_SOCKET;
	}

	return false;
}
