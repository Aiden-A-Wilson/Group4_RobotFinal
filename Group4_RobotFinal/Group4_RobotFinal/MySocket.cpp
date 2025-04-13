#include "MySocket.h"

MySocket::MySocket(SocketType socketType, std::string ip, unsigned int port, ConnectionType connType, unsigned int bufferSize) {
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

	int sock = connectionType == TCP ? SOCK_STREAM : SOCK_DGRAM;
	int protocol = connectionType == TCP ? IPPROTO_TCP : IPPROTO_UDP;

	if (mySocket == SERVER) {
		

		if (connectionType == TCP) {
			WelcomeSocket = socket(AF_INET, sock, protocol);
			if (WelcomeSocket == INVALID_SOCKET) {
				std::cout << "Unable to create WelcomeSocket" << std::endl;
			}

			int binded = bind(WelcomeSocket, (struct sockaddr*)&SvrAddr, addrLen);
			if (binded == SOCKET_ERROR) {
				std::cout << "Unable to bind welcome socket" << std::endl;
			}

			int listened = listen(WelcomeSocket, 1);
			if (listened != SOCKET_ERROR) {
				std::cout << "TCP server waiting for connection..." << std::endl;

				ConnectionSocket = accept(WelcomeSocket, (struct sockaddr*)&SvrAddr, &addrLen);
				
				if (ConnectionSocket == INVALID_SOCKET) {
					bTCPConnect = false;
					std::cout << "Unable to accept connection for WelcomeSocket";
				}
				else {
					bTCPConnect = true;
					std::cout << "Found Client!" << std::endl;
				}
			}
			else {
				std::cout << "Unable to listen for connections" << std::endl;
			}
		}
		else if (connectionType == UDP) {
			ConnectionSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if (ConnectionSocket == INVALID_SOCKET) {
				std::cout << "Unable to create ConnectionSocket" << std::endl;
			}

			int binded = bind(ConnectionSocket, (struct sockaddr*)&SvrAddr, addrLen);
			if (binded == SOCKET_ERROR) {
				std::cout << "Unable to bind ConnectionSocket" << std::endl;
			}
		}
	}
	else if (mySocket == CLIENT) {
		ConnectionSocket = socket(AF_INET, sock, protocol);
		if (ConnectionSocket == INVALID_SOCKET) {
			std::cout << "Unable to create ConnectionSocket" << std::endl;
		}

		if (connectionType == UDP) {
			int binded = bind(ConnectionSocket, (struct sockaddr*)&SvrAddr, addrLen);
			if (binded == SOCKET_ERROR) {
				std::cout << "Unable to bind ConnectionSocket" << std::endl;
			}
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
	if (connectionType == UDP || bTCPConnect) {
		return;
	}

	int addrLen = sizeof(SvrAddr);
	int connected = connect(ConnectionSocket, (struct sockaddr*)&SvrAddr, addrLen);
	bTCPConnect = true;
	if (connected == SOCKET_ERROR) {
		bTCPConnect = false;
		std::cout << "Unable to connect to TCP server with ConnectionSocket" << std::endl;
	}
}

void MySocket::DisconnectTCP() {
	if (connectionType == UDP) {
		return;
	}

	closesocket(ConnectionSocket);
	bTCPConnect = false;
}

bool MySocket::IsConnectedTCP() {
	return bTCPConnect;
}

void MySocket::SendData(const char* data, int size) {
	if (connectionType == TCP) {
		send(ConnectionSocket, data, size, 0);
	}
	else if (connectionType == UDP) {
		int addrLen = sizeof(SvrAddr);
		int s = sendto(ConnectionSocket, data, size, 0, (struct sockaddr*)&SvrAddr, addrLen);
	}
}

int MySocket::GetData(char* RxBuffer) {
	int bytesWritten = 0;
	if (connectionType == TCP) {
		bytesWritten = recv(ConnectionSocket, Buffer, MaxSize, 0);
	}
	else if (connectionType == UDP) {
		struct sockaddr_in senderAddr;
		int addrLen = sizeof(senderAddr);
		bytesWritten = recvfrom(ConnectionSocket, Buffer, MaxSize, 0, (struct sockaddr*)&senderAddr, &addrLen);
	}

	if (bytesWritten > 0) {
		memcpy(RxBuffer, Buffer, bytesWritten);
	}

	return bytesWritten;
}
std::string MySocket::GetIPAddr() {
	return std::to_string(SvrAddr.sin_addr.s_addr);
}
void MySocket::SetIPAddr(std::string ip) {
	if (CanSet()) {
		SvrAddr.sin_addr.s_addr = inet_addr(ip.c_str());
	}
	else {
		std::cout << "cannot change ip address socket is already open or connected" << std::endl;
	}
}

void MySocket::setPort(int port) {
	if (CanSet()) {
		Port = port;
		SvrAddr.sin_port = htons(port);
	}
	else {
		std::cout << "cannot change port socket is already open or connected" << std::endl;
	}
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
		std::cout << "cannot change type socket is already open or connected" << std::endl;
}

bool MySocket::CanSet()
{
	if (connectionType == TCP) {
		return !bTCPConnect;
	}

	return ConnectionSocket == INVALID_SOCKET;
}
