#include "scaner.h"
#include <iostream>
#include <string>

using namespace std;

void scaner::parseArguments(int argc, char* argv[]) {
    string interface, tcpPorts, udpPorts;
    int timeout = 5000;

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];

        if (arg == "-i" || arg == "--interface") {
            if (i + 1 < argc) interface = argv[++i];
        }
        else if (arg == "-t" || arg == "--pt") {
            if (i + 1 < argc) tcpPorts = argv[++i];
        }
        else if (arg == "-u" || arg == "--pu") {
            if (i + 1 < argc) udpPorts = argv[++i];
        }
        else if (arg == "-w" || arg == "--wait") {
            if (i + 1 < argc) timeout = stoi(argv[++i]);
        }
        else {
            cerr << "Unknown argument: " << arg << endl;
        }
    }
}