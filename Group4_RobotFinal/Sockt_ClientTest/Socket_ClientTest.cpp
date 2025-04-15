// Sockt_ClientTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "MySocket.h"
#include <Windows.h>

using namespace std;

int main(int argc, char* argv[])
{
    // WSA Startup
    WSADATA wsaData;
    int startup = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (startup != 0) {
        cout << "ERROR: Failed to start WSA." << endl;
        exit(EXIT_FAILURE);
    }

    Sleep(1000);

    // Determine connection type (TCP or UDP)

    ConnectionType conn = TCP;
    if (strcmp(argv[2], "TCP") == 0) {
        conn = TCP;
    }
    else if (strcmp(argv[2], "UDP") == 0) {
        conn = UDP;
    }

    // Create a client socket with the given port, IP, and connection type
    MySocket client = MySocket(CLIENT, "127.0.0.1", stoi(argv[1]), conn, 1024);

    // Connect or bind based on connection type
    if (conn == TCP) {
        client.ConnectTCP();
    }
    else {
        client.Bind();
    }

    // If "get_message" command is provided, send a message
    if (argc > 3 && strcmp(argv[3], "get_message") == 0) {
        char message[] = "Hello World!";
        client.SendData(message, 13);
        cout << "Sent message to client" << endl;
    }

    // If "send_message" command is provided, receive and respond
    if (argc > 3 && strcmp(argv[3], "send_message") == 0) {
        char buffer[1024] = { 0 };
        client.GetData(buffer);

        // If the message received matches expected string, send confirmation
        if (strcmp(buffer, "Message from server") == 0) {
            char message[] = "Confirmed";
            client.SendData(message, 10);
            cout << "Sent message to server" << endl;
        }
    }
    // Cleanup
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
