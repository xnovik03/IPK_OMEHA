#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <unistd.h>  // pro getopt
#include <cstring>   // pro strncpy
#include "NetworkInterface.h"
#include "PortRangeParser.h"
#include "TCPscanner.h"
#include "UDPscanner.h"

// Validace argumentů
bool validateArguments(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "ERROR: Invalid number of arguments. Usage: ./ipk-l4-scan -i <interface> -t <port>" << std::endl;
        return false;
    }
    return true;
}

// Zpracování argumentů z příkazové řádky
void processArguments(int argc, char* argv[], std::string &interfaceName, std::string &tcpPorts, std::string &udpPorts, int &timeout) {
    bool interfaceProvided = false;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-i" || arg == "--interface") {
            if (i + 1 < argc) {
                interfaceName = argv[i + 1];
                std::cout << "Selected interface: " << interfaceName << std::endl;
                interfaceProvided = true;
                i++; 
            }
            else {
                std::cout << "ERROR: Missing interface name" << std::endl;
                return;
            }
        }
        else if (arg == "--pt" || arg == "-t") {
            if (i + 1 < argc) {
                tcpPorts = argv[i + 1];
                std::cout << "TCP Ports: " << tcpPorts << std::endl;
                i++;
            }
        }
        else if (arg == "--pu" || arg == "-u") {
            if (i + 1 < argc) {
                udpPorts = argv[i + 1];
                std::cout << "UDP Ports: " << udpPorts << std::endl;
                i++;
            }
        }
        else if (arg == "--wait" || arg == "-w") {
            if (i + 1 < argc) {
                timeout = std::stoi(argv[i + 1]);
                std::cout << "Timeout set to: " << timeout << " ms" << std::endl;
                i++;
            }
        }
    }
    if (!interfaceProvided) {
        std::cout << "ERROR: Network interface not provided" << std::endl;
    }
}

// Hlavní funkce
int main(int argc, char* argv[]) {
    if (!validateArguments(argc, argv)) {
        return 1;
    }

    std::string interfaceName, tcpPorts, udpPorts;
    int timeout = 5000;

    // Zpracování argumentů
    processArguments(argc, argv, interfaceName, tcpPorts, udpPorts, timeout);

    // Převod TCP portů na čísla
    std::vector<int> tcpPortNumbers = PortRangeParser::parsePortRanges(tcpPorts);
    for (int port : tcpPortNumbers) {
        TCPScanner::scanPort("localhost", port);  // Místo localhost použijte IP
    }

    // Převod UDP portů na čísla
    std::vector<int> udpPortNumbers = PortRangeParser::parsePortRanges(udpPorts);
    for (int port : udpPortNumbers) {
        UDPScanner::scanPort("localhost", port);  // Místo localhost použijte IP
    }

    return 0;
}
