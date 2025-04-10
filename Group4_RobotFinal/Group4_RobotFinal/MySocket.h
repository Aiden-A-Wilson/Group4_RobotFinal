#pragma once

#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <string>
#include <cstring> 

#define DEFAULT_SIZE 1024

enum SocketType {CLIENT, SERVER};
enum ConnectionType {TCP, UDP};

class MySocket
{
	char* Buffer;
	SOCKET WelcomeSocket;
	SOCKET ConnectionSocket;
	struct sockaddr_in SvrAddr;
	SocketType mySocket;
	ConnectionType connectionType;
	std::string IPAddr;
	int Port;
	bool bTCPConnect;
	int MaxSize;

public:
	MySocket(SocketType, std::string, unsigned int, ConnectionType, unsigned int);
	~MySocket();
	void ConnectTCP();
	void DisconnectTCP();
	void SendData(const char*, int);
	int GetData(char*);
	std::string GetIPAddr();
	void SetIPAddr(std::string);
	void setPort(int);
	int GetPort();
	SocketType GetType();
	void SetType(SocketType);
};

