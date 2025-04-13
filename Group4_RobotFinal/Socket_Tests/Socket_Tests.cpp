#include "pch.h"
#include "CppUnitTest.h"
#include "MySocket.h"
#include <windows.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

void WinsockStart();
void RunTcpServer(PROCESS_INFORMATION* pi, std::wstring commands);
void CloseTcpServer(PROCESS_INFORMATION* pi);

namespace SocketTests
{
	TEST_CLASS(SocketTests)
	{
	public:

		TEST_METHOD(Client_TCP_CanConnectToServer)
		{
			PROCESS_INFORMATION pi;
			RunTcpServer(&pi, L" 7770 TCP");

			MySocket client = MySocket(CLIENT, "127.0.0.1", 7770, TCP, 1024);
			client.ConnectTCP();
			
			Assert::IsTrue(client.IsConnectedTCP());

			CloseTcpServer(&pi);
		}

		TEST_METHOD(Client_TCP_CanGetMessageFromServer)
		{
			PROCESS_INFORMATION pi;
			RunTcpServer(&pi, L" 7771 TCP get_message");

			MySocket client = MySocket(CLIENT, "127.0.0.1", 7771, TCP, 1024);
			client.ConnectTCP();

			char buffer[1024] = { 0 };
			client.GetData(buffer);

			Assert::AreEqual(0, strcmp(buffer, "Hello World!"));

			CloseTcpServer(&pi);
		}

		TEST_METHOD(Client_UDP_CanGetMessageFromServer)
		{
			WinsockStart();

			PROCESS_INFORMATION pi;
			RunTcpServer(&pi, L" 7772 UDP get_message");

			MySocket client = MySocket(CLIENT, "127.0.0.1", 7772, UDP, 1024);

			char buffer[1024] = { 0 };
			client.GetData(buffer);
			Assert::AreEqual(0, strcmp(buffer, "Hello World!"));

			CloseTcpServer(&pi);
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

void RunTcpServer(PROCESS_INFORMATION* pi, std::wstring commands) {
	STARTUPINFO si = { sizeof(si) };

	// Copy string to modifiable buffer
	std::wstring command = L"cmd.exe /k \"Socket_ServerTest.exe\"";
	command.append(commands);

	BOOL success = CreateProcess(
		nullptr,                      // Application name
		&command[0],                  // Writable command line
		nullptr, nullptr,
		FALSE,
		CREATE_NEW_CONSOLE,          // Show console
		nullptr,
		nullptr,
		&si, pi);
}

void CloseTcpServer(PROCESS_INFORMATION* pi) {
	TerminateProcess(pi->hProcess, 0);
	CloseHandle(pi->hProcess);
	CloseHandle(pi->hThread);
}