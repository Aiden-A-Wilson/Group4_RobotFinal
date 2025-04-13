// Group4_RobotFinal.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")
#include "PktDef.h"
#include <iomanip>
#include "MySocket.h"

using namespace std;

int main()
{
    
    // WSA Startup
    WSADATA wsaData;
    int startup = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (startup != 0) {
        cout << "ERROR: Failed to start WSA." << endl;
        exit(EXIT_FAILURE);
    }

    // Socket
    MySocket client = MySocket(CLIENT, "127.0.0.1", 7777, UDP, 1024);

    // Send and Receive
    PktDef packet = PktDef();
    packet.SetPktCount(5);
    packet.SetCmd(RESPONSE);
    struct DriveBody body = { 0 };
    body.Direction = 1;
    body.Duration = 10;
    body.Speed = 80;
    packet.SetBodyData((char*)&body, BODYSIZE);

    char* sBuffer = packet.GenPacket();
    int totalSize = packet.GetLength();
    client.SendData(sBuffer, totalSize);

    char buffer[1024] = { 0 };
    int bytesRead = client.GetData(buffer);
    cout << bytesRead << endl;

    WSACleanup();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
