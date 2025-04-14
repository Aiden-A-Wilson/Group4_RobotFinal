#define CROW_MAIN

#include "crow_all.h"
#include <fstream>
#include "MySocket.h"

using namespace std;
using namespace crow;

void SendFile(response& res, string fileName, string contentType);
void SendHtml(response& res, string fileName);

int main() {
    SimpleApp app;
    MySocket* socket = nullptr;

    // index
    CROW_ROUTE(app, "/")
    ([](const request& req, response& res) {
        SendHtml(res, "index.html");
    });

    CROW_ROUTE(app, "/connect/<string>/<int>").methods(HTTPMethod::POST)
    ([&](const request& req, response& res, string ip, int port) {
        socket = new MySocket(CLIENT, ip, port, UDP, 1024);
    });

    CROW_ROUTE(app, "/telecommand").methods(HTTPMethod::PUT)
    ([](const request& req, response& res) {
        
    });

    CROW_ROUTE(app, "/telemetry_request").methods(HTTPMethod::GET)
    ([](const request& req, response& res) {
        
    });

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