#define CROW_MAIN

#include "crow_all.h"
#include <fstream>
#include <unordered_map>

using namespace std;
using namespace crow;

int main() {
    SimpleApp app;

    // index
    CROW_ROUTE(app, "/")
        ([](const request& req, response& res) {
        ifstream in("../public/index.html", ifstream::in);
        if (in) {
            ostringstream contents;
            contents << in.rdbuf();
            in.close();
            res.write(contents.str());
        }
        else {
            res.write("Not Found");
        }
        res.end();
            });
}