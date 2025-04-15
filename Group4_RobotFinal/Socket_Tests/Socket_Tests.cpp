#include "pch.h"
#include "CppUnitTest.h"
#include "MySocket.h"
#include <windows.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

void WinsockStart();
void RunServer(PROCESS_INFORMATION* pi, std::wstring commands);
void RunClient(PROCESS_INFORMATION* pi, std::wstring commands);
void CloseProcess(PROCESS_INFORMATION* pi);

/*
* NOTE: Please make sure the ENTIRE solution is built before running these tests.
* These tests rely on an external application so make sure you antivirus does not flag it.
*/

namespace SocketTests
{
	TEST_CLASS(SocketTests)
	{
	public:

		TEST_METHOD(ConnectTCP_Client_TCP_ConnectsToRunningServer)
		{
			// Arrange
			WinsockStart();
			PROCESS_INFORMATION pi;
			RunServer(&pi, L"7770 TCP");
			MySocket client = MySocket(CLIENT, "127.0.0.1", 7770, TCP, 1024);

			// Act
			client.ConnectTCP();
			
			// Assert
			Assert::IsTrue(client.IsConnectedTCP());

			// Cleanup
			CloseProcess(&pi);
			WSACleanup();
		}

		TEST_METHOD(GetData_Client_TCP_ObtainsBytesFromServer)
		{
			// Arrange
			WinsockStart();
			PROCESS_INFORMATION pi;
			RunServer(&pi, L"7771 TCP get_message");
			MySocket client = MySocket(CLIENT, "127.0.0.1", 7771, TCP, 1024);
			client.ConnectTCP();
			char buffer[1024] = { 0 };

			// Act
			client.GetData(buffer);

			// Assert
			Assert::AreEqual(0, strcmp(buffer, "Hello World!"));

			// Cleanup
			CloseProcess(&pi);
			WSACleanup();
		}

		TEST_METHOD(GetData_Client_UDP_ObtainsBytesFromServer)
		{
			// Arrange
			WinsockStart();
			PROCESS_INFORMATION pi;
			RunServer(&pi, L"7772 UDP get_message");
			MySocket client = MySocket(CLIENT, "127.0.0.1", 7772, UDP, 1024);
			client.Bind();
			char buffer[1024] = { 0 };
			
			// Act
			client.GetData(buffer);
			
			// Assert
			Assert::AreEqual(0, strcmp(buffer, "Hello World!"));

			// Cleanup
			CloseProcess(&pi);
			WSACleanup();
		}

		TEST_METHOD(Constructor_Client_TCP_CreatesTcpClientSocket)
		{
			// Arrange
			WinsockStart();

			// Act
			MySocket client = MySocket(CLIENT, "127.0.0.1", 7773, TCP, 1024);

			// Assert
			Assert::AreEqual((int)CLIENT, (int)client.GetType());
			Assert::AreEqual(std::string("127.0.0.1"), client.GetIPAddr());
			Assert::AreEqual(7773, client.GetPort());

			// Cleanup
			WSACleanup();
		}

		TEST_METHOD(Constructor_Client_UDP_CreatesUdpClientSocket)
		{
			// Arrange
			WinsockStart();

			// Act
			MySocket client = MySocket(CLIENT, "127.0.0.1", 7774, UDP, 1024);

			// Assert
			Assert::AreEqual((int)CLIENT, (int)client.GetType());
			Assert::AreEqual(std::string("127.0.0.1"), client.GetIPAddr());
			Assert::AreEqual(7774, client.GetPort());

			// Cleanup
			WSACleanup();
		}

		TEST_METHOD(Constructor_Server_UDP_CreatesUdpServerSocket)
		{
			// Arrange
			WinsockStart();

			// Act
			MySocket client = MySocket(SERVER, "127.0.0.1", 7775, UDP, 1024);

			// Assert
			Assert::AreEqual((int)SERVER, (int)client.GetType());
			Assert::AreEqual(std::string("127.0.0.1"), client.GetIPAddr());
			Assert::AreEqual(7775, client.GetPort());

			// Cleanup
			WSACleanup();
		}

		TEST_METHOD(Constructor_Server_TCP_CreatesTcpServerSocket)
		{
			// Arrange
			WinsockStart();
			PROCESS_INFORMATION pi;
			RunClient(&pi, L"7776 TCP");

			// Act
			MySocket server = MySocket(SERVER, "127.0.0.1", 7776, TCP, 1024);

			// Assert
			Assert::AreEqual((int)SERVER, (int)server.GetType());
			Assert::AreEqual(std::string("127.0.0.1"), server.GetIPAddr());
			Assert::AreEqual(7776, server.GetPort());

			// Cleanup
			CloseProcess(&pi);
			WSACleanup();
		}

		TEST_METHOD(SendData_Client_TCP_SendsBytesToServer)
		{
			// Arrange
			WinsockStart();
			PROCESS_INFORMATION pi;
			RunServer(&pi, L"7777 TCP send_message");
			MySocket client = MySocket(CLIENT, "127.0.0.1", 7777, TCP, 1024);
			client.ConnectTCP();
			char buffer[1024] = { 0 };

			// Act
			client.SendData("Message from client", 20);

			// Assert
			client.GetData(buffer);
			Assert::AreEqual(0, strcmp(buffer, "Confirmed"));

			// Cleanup
			CloseProcess(&pi);
			WSACleanup();
		}

		TEST_METHOD(SendData_Client_UDP_SendsBytesToServer)
		{
			// Arrange
			WinsockStart();
			PROCESS_INFORMATION pi;
			RunServer(&pi, L"7778 UDP send_message");
			Sleep(2000);
			MySocket client = MySocket(CLIENT, "127.0.0.1", 7778, UDP, 1024);
			char buffer[1024] = { 0 };

			// Act
			client.SendData("Message from client", 20);
			
			// Assert
			client.GetData(buffer);
			Assert::AreEqual(0, strcmp(buffer, "Confirmed"));

			// Cleanup
			CloseProcess(&pi);
			WSACleanup();
		}

		TEST_METHOD(GetPort_ReturnsCorrectPort)
		{
			MySocket client = MySocket(CLIENT, "127.0.0.1", 7779, TCP, 1024);

			//assert
			Assert::AreEqual(7779, client.GetPort());
		}

		TEST_METHOD(SetPort_ChangesPort_WhenNotConnected)
		{
			MySocket client = MySocket(CLIENT, "127.0.0.1", 7780, TCP, 1024);

			//assert
			client.setPort(23000);
			Assert::AreEqual(23000, client.GetPort());
		}

		TEST_METHOD(SetPort_DoesNotChangePort_WhenConnected)
		{
			//arrange
			PROCESS_INFORMATION pi;
			RunServer(&pi, L"7781 TCP");
			MySocket client = MySocket(CLIENT, "127.0.0.1", 7781, TCP, 1024);

			//act
			client.ConnectTCP();
			client.setPort(9999);

			//assert
			Assert::AreEqual(7781, client.GetPort());
			CloseProcess(&pi);
		}

		TEST_METHOD(GetType_ReturnsCorrectType_Client)
		{
			MySocket client = MySocket(CLIENT, "127.0.0.1", 7782, TCP, 1024);

			//assert
			Assert::IsTrue(client.GetType() == CLIENT);
		}

		TEST_METHOD(GetType_ReturnsCorrectType_Server)
		{
			MySocket server = MySocket(SERVER, "127.0.0.1", 7783, UDP, 1024);

			//assert
			Assert::IsTrue(server.GetType() == SERVER);
		}

		TEST_METHOD(SetType_ChangesType_WhenNotConnected)
		{
			MySocket socket = MySocket(CLIENT, "127.0.0.1", 7784, TCP, 1024);
			socket.SetType(SERVER);

			//assert
			Assert::IsTrue(socket.GetType() == SERVER);
		}

		TEST_METHOD(SetType_DoesNotChangeType_WhenConnected)
		{
			//arrange
			PROCESS_INFORMATION pi;
			RunServer(&pi, L"7785 TCP");
			MySocket client = MySocket(CLIENT, "127.0.0.1", 7785, TCP, 1024);

			//act
			client.ConnectTCP();
			client.SetType(SERVER);

			//assert
			Assert::IsTrue(client.GetType() == CLIENT);
			CloseProcess(&pi);
		}

		TEST_METHOD(GetIP_ReturnsCorrectIP)
		{
			//arrange
			MySocket client = MySocket(CLIENT, "127.0.0.1", 7786, TCP, 1024);

			//act
			std::string expected = "127.0.0.1";

			//assert
			Assert::AreEqual(client.GetIPAddr(), expected);
		}

		TEST_METHOD(SetIP_ChangesIP_WhenNotConnected)
		{
			//arrange
			MySocket socket = MySocket(CLIENT, "127.0.0.1", 7787, TCP, 1024);

			//act
			socket.SetIPAddr("138.212.87.72");
			std::string expected = "138.212.87.72";

			//assert
			Assert::AreEqual(socket.GetIPAddr(), expected);
		}

		TEST_METHOD(SetIP_DoesNotChangeIP_TCP_WhenConnected)
		{
			//arrange
			PROCESS_INFORMATION pi;
			RunServer(&pi, L"7788 TCP");

			//act
			MySocket client = MySocket(CLIENT, "127.0.0.1", 7788, TCP, 1024);
			client.ConnectTCP();

			client.SetIPAddr("138.212.87.74");
			std::string expected = "127.0.0.1";

			//assert
			Assert::AreEqual(client.GetIPAddr(), expected);
			CloseProcess(&pi);
		}

		TEST_METHOD(SetIP_ChangesIP_UDP_WhenConnected)
		{
			//arrange
			MySocket client = MySocket(CLIENT, "127.0.0.1", 7789, UDP, 1024);

			//act
			client.SetIPAddr("138.212.87.75");
			std::string expected = "138.212.87.75";

			//assert
			Assert::AreEqual(client.GetIPAddr(), expected);
		}

		TEST_METHOD(SendData_Server_TCP_SendsBytesToClient)
		{
			// Arrange
			WinsockStart();
			PROCESS_INFORMATION pi;
			RunClient(&pi, L"7790 TCP send_message");
			MySocket server = MySocket(SERVER, "127.0.0.1", 7790, TCP, 1024);
			char buffer[1024] = { 0 };

			// Act
			server.SendData("Message from server", 20);

			// Assert
			server.GetData(buffer);
			Assert::AreEqual(0, strcmp(buffer, "Confirmed"));

			// Cleanup
			CloseProcess(&pi);
			WSACleanup();
		}

		TEST_METHOD(SendData_Server_UDP_SendsBytesToClient)
		{
			// Arrange
			WinsockStart();
			PROCESS_INFORMATION pi;
			RunClient(&pi, L"7791 UDP send_message");
			Sleep(2000);
			MySocket server = MySocket(SERVER, "127.0.0.1", 7791, UDP, 1024);
			char buffer[1024] = { 0 };

			// Act
			server.SendData("Message from server", 20);

			// Assert
			server.GetData(buffer);
			Assert::AreEqual(0, strcmp(buffer, "Confirmed"));

			// Cleanup
			CloseProcess(&pi);
			WSACleanup();
		}
	};
}

void WinsockStart() {
	// WSA Startup
	WSADATA wsaData;
	int startup = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (startup != 0) {
		std::cout << "ERROR: Failed to start WSA." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void RunServer(PROCESS_INFORMATION* pi, std::wstring commands) {
	STARTUPINFO si = { sizeof(si) };

	std::wstring command = L"cmd.exe /k \"Socket_ServerTest.exe\"";
	command.append(L" ");
	command.append(commands);

	BOOL success = CreateProcess(
		nullptr,
		&command[0],
		nullptr, nullptr,
		FALSE,
		CREATE_NEW_CONSOLE,
		nullptr,
		nullptr,
		&si, pi);
}

void RunClient(PROCESS_INFORMATION* pi, std::wstring commands) {
	STARTUPINFO si = { sizeof(si) };

	std::wstring command = L"cmd.exe /k \"Socket_ClientTest.exe\"";
	command.append(L" ");
	command.append(commands);

	BOOL success = CreateProcess(
		nullptr,
		&command[0],
		nullptr, nullptr,
		FALSE,
		CREATE_NEW_CONSOLE,
		nullptr,
		nullptr,
		&si, pi);
}

void CloseProcess(PROCESS_INFORMATION* pi) {
	TerminateProcess(pi->hProcess, 0);
	CloseHandle(pi->hProcess);
	CloseHandle(pi->hThread);
}