#define CROW_MAIN
#define BUFFER_SIZE 1024

#include "crow_all.h"
#include <fstream>
#include "MySocket.h"
#include "PktDef.h"

using namespace std;
using namespace crow;

void SendFile(response& res, string fileName, string contentType);
void SendHtml(response& res, string fileName);
int SendAndReceive(MySocket* socket, PktDef& packet, char* rcvBuffer);

int main() {
    SimpleApp app;
    MySocket* socket = nullptr;
    char RxBuffer[BUFFER_SIZE] = { 0 };
    int packetCount = 1;

    // index
    CROW_ROUTE(app, "/")
    ([](const request& req, response& res) {
        SendHtml(res, "index.html");
    });

    CROW_ROUTE(app, "/connect/<string>/<int>").methods(HTTPMethod::POST)
    ([&](const request& req, response& res, string ip, int port) {
        if (socket != nullptr) {
            delete socket;
            socket = nullptr;
        }

        socket = new MySocket(CLIENT, ip, port, UDP, 1024);
        cout << "Created new UDP socket" << endl;
    });

    CROW_ROUTE(app, "/telecommand").methods(HTTPMethod::PUT)
    ([&](const request& req, response& res) {
        if (socket == nullptr) {
            return;
        }

        string command = req.url_params.get("command");
        if (command.empty()) {
            res.code = 404;
			res.end();
			return;
        }

        PktDef packet = PktDef();
        int bytesReceived = 0;
        memset(RxBuffer, 0, BUFFER_SIZE);

        if (command == "DRIVE") {
            auto direction = req.url_params.get("direction");
            auto duration = req.url_params.get("duration");
            auto speed = req.url_params.get("speed");
            if (!direction || !duration || !speed) {
                res.code = 404;
                res.end();
                return;
            }

            packet.SetCmd(DRIVE);
            packet.SetPktCount(packetCount);
            struct DriveBody body = { 0 };
            body.Direction = atoi(direction);
            body.Duration = atoi(duration);
            body.Speed = atoi(speed);
            packet.SetBodyData((char*)&body, sizeof(body));

            bytesReceived = SendAndReceive(socket, packet, RxBuffer);
        }
        else if (command == "SLEEP") {
            packet.SetCmd(SLEEP);
            packet.SetPktCount(packetCount);
            bytesReceived = SendAndReceive(socket, packet, RxBuffer);
            res.code = 200;
            res.write("Program ended");
            res.end();
            return;
        }

        if (bytesReceived > 0) {
            PktDef received = PktDef(RxBuffer);
            if (!received.GetAck()) {
                res.code = 404;
                res.write("Nack");
                res.end();
                return;
            }

            packetCount = received.GetPktCount();
            cout << "Packet Acknowledged" << endl;
            res.code = 200;
            res.write("Packet Acknowledged");
        } else {
            res.code = 404;
            res.write("No bytes received");
        }

        res.end();
    });

    CROW_ROUTE(app, "/telemetry_request").methods(HTTPMethod::GET)
    ([&](const request& req, response& res) {
        if (socket == nullptr) {
            return;
        }

        PktDef packet = PktDef();
        int bytesReceived = 0;
        memset(RxBuffer, 0, BUFFER_SIZE);

        packet.SetCmd(RESPONSE);
        packet.SetPktCount(packetCount);

        bytesReceived = SendAndReceive(socket, packet, RxBuffer);

        if (bytesReceived > 0) {
            PktDef received = PktDef(RxBuffer);
            if (!received.GetAck()) {
                res.code = 404;
                res.write("Nack");
                res.end();
                return;
            }

            memset(RxBuffer, 0, BUFFER_SIZE);
            int telemBytes = socket->GetData(RxBuffer);

            if (telemBytes <= 0) {
                res.code = 404;
                res.write("No bytes received for telem packet.");
                res.end();
                return;
            }
            
            PktDef telemPacket = PktDef(RxBuffer);
            struct TelemetryResponse telem = { 0 };
            memcpy(&telem, telemPacket.GetBodyData(), sizeof(telem));
            packetCount = telemPacket.GetPktCount();
            
            ostringstream stream = ostringstream();
            stream << "Packet Acknowledged" << "\n";
            stream << "Last Packet Counter: " << to_string(telem.LastPktCounter) << "\n";
            stream << "Current Grade: " << to_string(telem.CurrentGrade) << "\n";
            stream << "Hit Count: " << to_string(telem.HitCount) << "\n";
            stream << "Last Command: " << to_string(telem.LastCmd) << "\n";
            stream << "Last Command Value: " << to_string(telem.LastCmdValue) << "\n";
            stream << "Last Command Speed: " << to_string(telem.LastCmdSpeed) << "\n";
            
            res.code = 200;
            res.write(stream.str());
        } else {
            res.code = 404;
            res.write("No bytes received");
        }

        res.end();
    });

    app.port(7770).multithreaded().run();

    if (socket != nullptr) {
        delete socket;
    }

    return 0;
}

void SendFile(response& res, string fileName, string contentType) {
    ifstream in("../public/" + fileName, ifstream::in);
    if (in) {
        ostringstream contents;
        contents << in.rdbuf();
        in.close();
        res.set_header("Content-Type", contentType);
        res.write(contents.str());
    }
    else {
        res.write("Not Found");
    }
    res.end();
}

void SendHtml(response &res, string fileName) {
    SendFile(res, fileName, "text/html");
}

int SendAndReceive(MySocket* socket, PktDef& packet, char* rcvBuffer) {
    socket->SendData(packet.GenPacket(), packet.GetLength());
    int bytesReceived = socket->GetData(rcvBuffer);
    return bytesReceived;
}