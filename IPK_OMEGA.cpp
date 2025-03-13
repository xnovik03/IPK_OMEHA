#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <unistd.h>  // pro getopt
#include <cstring>   // pro strncpy
#include "NetworkInterface.h"


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
    std::vector<int> tcpPorts;
    std::vector<int> udpPorts;
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
                std::string ports = argv[i + 1];
               
                std::cout << "TCP Ports: " << ports << std::endl;
                i++;
            }
        }
        else if (arg == "--pu" || arg == "-u") {
            if (i + 1 < argc) {
                std::string ports = argv[i + 1];
                std::cout << "UDP Ports: " << ports << std::endl;
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
        interfaces = NetworkInterface::getAvailableInterfaces();
        if (interfaces.empty()) {
            std::cout << "No network interfaces found!" << std::endl;
            return;
        }
        NetworkInterface::selectInterface(interfaces);
    }

  
}

int main(int argc, char* argv[]) {
   
    if (!validateArguments(argc, argv)) {
        return 1;
    }

    processArguments(argc, argv);

    return 0;
}
