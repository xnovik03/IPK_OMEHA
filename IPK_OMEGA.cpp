#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <unistd.h>  // pro getopt
#include <cstring>   // pro strncpy
#include "NetworkInterface.h"
#include "PortRangeParser.h"

bool validateArguments(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "ERROR: Invalid number of arguments. Usage: ./ipk-l4-scan -i <interface> -t <port>" << std::endl;
        return false;
    }
    return true;
}

void processArguments(int argc, char* argv[]) {
    std::vector<std::string> interfaces;
    bool interfaceProvided = false;
    std::string interfaceName;
    std::string portRange;  // Proměnná pro uchování portů
    int timeout = 5000;

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
                portRange = argv[i + 1];  // Uložení portů do proměnné
                std::cout << "TCP Ports: " << portRange << std::endl;
                i++;
            }
        }
        else if (arg == "--pu" || arg == "-u") {
            if (i + 1 < argc) {
                portRange = argv[i + 1];  // Uložení UDP portů
                std::cout << "UDP Ports: " << portRange << std::endl;
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

    // Pokud nebyl zadaný interface, vybíráme první dostupný
    if (!interfaceProvided) {
        interfaces = NetworkInterface::getAvailableInterfaces();
        if (interfaces.empty()) {
            std::cout << "No network interfaces found!" << std::endl;
            return;
        }
        NetworkInterface::selectInterface(interfaces);
    }

    // Zpracování portRange pro TCP/UDP
    if (!portRange.empty()) {
        std::vector<int> ports = PortRangeParser::parsePortRanges(portRange);
        std::cout << "Selected Ports: ";
        for (int port : ports) {
            std::cout << port << " ";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (!validateArguments(argc, argv)) {
        return 1;
    }

    processArguments(argc, argv);
    return 0;
}
