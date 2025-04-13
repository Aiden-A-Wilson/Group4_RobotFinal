#pragma once

#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <string>
#include <cstring> 

#define DEFAULT_SIZE 1024

/// <summary>
/// The type of socket.
/// </summary>
enum SocketType {CLIENT, SERVER};

/// <summary>
/// The type of socket connection.
/// </summary>
enum ConnectionType {TCP, UDP};

/// <summary>
/// Represents a configurable networks socket (TCP/UDP).
/// </summary>
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
	struct sockaddr_in LastSender;
	int LastSenderLength;

public:
	/// <summary>
	/// Creates a new network socket with the specified information.
	/// </summary>
	/// <param name="Socket Type (Client or Server)"></param>
	/// <param name="IP Address"></param>
	/// <param name="Port"></param>
	/// <param name="Connection Type (TCP or UDP)"></param>
	/// <param name="Max Buffer Size"></param>
	MySocket(SocketType, std::string, unsigned int, ConnectionType, unsigned int);

	~MySocket();
	
	/// <summary>
	/// Connections a TCP client to a running TCP server.
	/// </summary>
	void ConnectTCP();

	/// <summary>
	/// Disconnects a running TCP client from the server it is connected to.
	/// </summary>
	void DisconnectTCP();

	/// <summary>
	/// Returns whether a TCP client or server has performed a three way handshake.
	/// </summary>
	/// <returns>Whether a TCP client or server is connected.</returns>
	bool IsConnectedTCP();

	/// <summary>
	/// Sends data to its specified server or IP:Port
	/// </summary>
	/// <param name="Reference to byte array."></param>
	/// <param name="Number of bytes to send."></param>
	void SendData(const char*, int);

	/// <summary>
	/// Receives bytes from the sender and stores it into an internal and external buffer.
	/// </summary>
	/// <param name="The external buffer to store the received bytes in."></param>
	/// <returns>The number of bytes received.</returns>
	int GetData(char*);

	/// <summary>
	/// Gets the IP Address of the socket.
	/// </summary>
	/// <returns>The IP Address of the socket.</returns>
	std::string GetIPAddr();

	/// <summary>
	/// Sets the IP Address of the socket.
	/// </summary>
	/// <param name="IP Address"></param>
	void SetIPAddr(std::string);

	/// <summary>
	/// Sets the port of the socket.
	/// </summary>
	/// <param name="Port"></param>
	void setPort(int);

	/// <summary>
	/// Gets the assigned port of the socket.
	/// </summary>
	/// <returns>The port of the socket</returns>
	int GetPort();

	/// <summary>
	/// Gets the socket type of the socket (Client or Server).
	/// </summary>
	/// <returns>The socket type of the socket.</returns>
	SocketType GetType();

	/// <summary>
	/// Sets the socket type of the socket (Client or Server).
	/// </summary>
	/// <param name="The socket type to set the socket to."></param>
	void SetType(SocketType);

private:
	/// <summary>
	/// Returns whether the socket is able to set values or not.
	/// </summary>
	/// <returns>If the socket is able to set values or not.</returns>
	bool CanSet();
};

