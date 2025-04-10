// Group4_RobotFinal.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")
#include "PktDef.h"
#include <iomanip>

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
    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (clientSocket == INVALID_SOCKET) {
        WSACleanup();
        cout << "Failed to create server socket" << endl;
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(7780);

    // Send and Receive
    PktDef packet = PktDef();
    packet.SetPktCount(5);
    packet.SetCmd(DRIVE);
    struct DriveBody body = { 0 };
    body.Direction = 1;
    body.Duration = 10;
    body.Speed = 80;
    packet.SetBodyData((char*)&body, BODYSIZE);

    int addressLength = sizeof(serverAddress);
    char* sBuffer = packet.GenPacket();
    int totalSize = packet.GetLength();
    sendto(clientSocket, sBuffer, totalSize, 0, (struct sockaddr*)&serverAddress, addressLength);
    cout << "Sent Packet with size: " << totalSize << endl;

    for (int i = 0; i < totalSize; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
            << (static_cast<unsigned int>(static_cast<unsigned char>(sBuffer[i]))) << " ";
    }
    std::cout << std::dec << std::endl;

    closesocket(clientSocket);
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
