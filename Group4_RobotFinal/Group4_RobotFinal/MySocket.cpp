#include "MySocket.h"

// Creates a new network socket with the specified information.
MySocket::MySocket(SocketType socketType, std::string ip, unsigned int port, ConnectionType connType, unsigned int bufferSize) {
	// Initialize member variables.
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

	LastSender.sin_family = AF_INET;
	LastSender.sin_addr.s_addr = inet_addr(ip.c_str());
	LastSender.sin_port = htons(port);
	LastSenderLength = sizeof(LastSender);

	// Initializing socket(s) based on the socket and connection types.
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
	}
	
	// Allocate new space for buffer.
	Buffer = new char[bufferSize];
}

// Cleanup
MySocket::~MySocket() {
	if (Buffer != nullptr) {
		delete[] Buffer;
	}

	closesocket(ConnectionSocket);
	closesocket(WelcomeSocket);
}

// Connections a TCP client to a running TCP server.
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

// Disconnects a running TCP client from the server it is connected to.
void MySocket::DisconnectTCP() {
	if (connectionType == UDP) {
		return;
	}

	closesocket(ConnectionSocket);
	bTCPConnect = false;
}

// Returns whether a TCP client or server has performed a three way handshake.
bool MySocket::IsConnectedTCP() {
	return bTCPConnect;
}

// Sends data to its specified server or IP:Port
void MySocket::SendData(const char* data, int size) {
	if (connectionType == TCP) {
		send(ConnectionSocket, data, size, 0);
	}
	else if (connectionType == UDP) {
		sendto(ConnectionSocket, data, size, 0, (struct sockaddr*)&LastSender, LastSenderLength);
	}
}

// Receives bytes from the sender and stores it into an internal and external buffer.
int MySocket::GetData(char* RxBuffer) {
	int bytesWritten = 0;
	if (connectionType == TCP) {
		bytesWritten = recv(ConnectionSocket, Buffer, MaxSize, 0);
	}
	else if (connectionType == UDP) {
		bytesWritten = recvfrom(ConnectionSocket, Buffer, MaxSize, 0, (struct sockaddr*)&LastSender, &LastSenderLength);
	}

	if (bytesWritten > 0) {
		memcpy(RxBuffer, Buffer, bytesWritten);
	}

	return bytesWritten;
}

// Gets the IP Address of the socket.
std::string MySocket::GetIPAddr() {
	return inet_ntoa(SvrAddr.sin_addr);
}

// Sets the IP Address of the socket.
void MySocket::SetIPAddr(std::string ip) {
	if (CanSet()) {
		SvrAddr.sin_addr.s_addr = inet_addr(ip.c_str());
	}
	else {
		std::cout << "cannot change ip address socket is already open or connected" << std::endl;
	}
}

// Sets the port of the socket.
void MySocket::setPort(int port) {
	if (CanSet()) {
		Port = port;
		SvrAddr.sin_port = htons(port);
	}
	else {
		std::cout << "cannot change port socket is already open or connected" << std::endl;
	}
}

// Gets the assigned port of the socket.
int MySocket::GetPort() {
	return Port;
}

// Gets the socket type of the socket (Client or Server).
SocketType MySocket::GetType() {
	return mySocket;
}

// Sets the socket type of the socket (Client or Server).
void MySocket::SetType(SocketType type) {
	if (CanSet()) {
		mySocket = type;
	}
	else
		std::cout << "cannot change type socket is already open or connected" << std::endl;
}

void MySocket::Bind() {
	if (connectionType == TCP) {
		return;
	}

	int binded = bind(ConnectionSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr));
	if (binded == SOCKET_ERROR) {
		std::cout << "Unable to bind ConnectionSocket" << std::endl;
	}
}

// Returns whether the socket is able to set values or not.
bool MySocket::CanSet()
{
	if (connectionType == TCP) {
		return !bTCPConnect;
	}

	return true;
}
