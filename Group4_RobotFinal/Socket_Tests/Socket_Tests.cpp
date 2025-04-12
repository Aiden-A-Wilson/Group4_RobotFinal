#include "pch.h"
#include "CppUnitTest.h"
#include "MySocket.h"
#include <windows.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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
			RunTcpServer(&pi, L" 7770");

			MySocket client = MySocket(CLIENT, "127.0.0.1", 7770, TCP, 1024);
			client.ConnectTCP();
			
			Assert::IsTrue(client.IsConnectedTCP());

			CloseTcpServer(&pi);
		}

		TEST_METHOD(Client_TCP_CanGetMessageFromServer)
		{
			PROCESS_INFORMATION pi;
			RunTcpServer(&pi, L" 7771 get_message");

			MySocket client = MySocket(CLIENT, "127.0.0.1", 7771, TCP, 1024);
			client.ConnectTCP();

			char buffer[1024] = { 0 };
			client.GetData(buffer);

			Assert::AreEqual(0, strcmp(buffer, "Hello World!"));

			CloseTcpServer(&pi);
		}
	};
}

void RunTcpServer(PROCESS_INFORMATION* pi, std::wstring commands) {
	STARTUPINFO si = { sizeof(si) };

	// Copy string to modifiable buffer
	std::wstring command = L"cmd.exe /k \"F:\\Conestoga Computer Science\\Semester 4\\Mobile and Networked Systems\\Group4_RobotFinal\\Group4_RobotFinal\\x64\\Debug\\Socket_ServerTest.exe\"";
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