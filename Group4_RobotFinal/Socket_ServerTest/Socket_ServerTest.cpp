// Socket_ServerTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "MySocket.h"

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

    // Set default connection type to TCP
    ConnectionType conn = TCP;

    // Check argument to determine if connection should be UDP
    if (strcmp(argv[2], "TCP") == 0) {
        conn = TCP;
    }
    else if (strcmp(argv[2], "UDP") == 0) {
        conn = UDP;
    }

    // Create a server socket using IP, port, connection type, and buffer size
    MySocket server = MySocket(SERVER, "127.0.0.1", stoi(argv[1]), conn, 1024);

    // If "get_message" command is passed, send a message
    if (argc > 3 && strcmp(argv[3], "get_message") == 0) {
        char message[] = "Hello World!";
        server.SendData(message, 13);
        cout << "Sent message to client" << endl;
    }
    // If "send_message" command is passed, receive message and respond if valid
    if (argc > 3 && strcmp(argv[3], "send_message") == 0) {
        char buffer[1024] = { 0 };
        server.GetData(buffer);

        // If received message matches expected string, send confirmation
        if (strcmp(buffer, "Message from client") == 0) {
            char message[] = "Confirmed";
            server.SendData(message, 10);
            cout << "Sent message to client" << endl;
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
