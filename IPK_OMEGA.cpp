#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <unistd.h>  // pro getopt
#include <cstring>   // pro strncpy
#include <netdb.h>   // pro getaddrinfo
#include <arpa/inet.h>
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
void processArguments(int argc, char* argv[], std::string &interfaceName, std::string &tcpPorts, std::string &udpPorts, int &timeout, std::string &host) {
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
        else {
            // Hostname or IP address (last argument)
            host = argv[i];
        }
    }
    if (!interfaceProvided) {
        std::cout << "ERROR: Network interface not provided" << std::endl;
    }
}

// Funkce pro získání IP adresy z hostname
std::vector<std::string> resolveHostToIP(const std::string &host) {
    std::vector<std::string> ipAddresses;
    struct addrinfo hints, *res;
    int status;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // Jak IPv4, tak IPv6
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(host.c_str(), nullptr, &hints, &res)) != 0) {
        std::cerr << "ERROR: Unable to resolve hostname: " << gai_strerror(status) << std::endl;
        return ipAddresses;
    }

    for (struct addrinfo *p = res; p != nullptr; p = p->ai_next) {
        void *addr;
        char ipStr[INET6_ADDRSTRLEN];

        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
        }

        inet_ntop(p->ai_family, addr, ipStr, sizeof(ipStr));
        ipAddresses.push_back(std::string(ipStr));
    }

    freeaddrinfo(res);
    return ipAddresses;
}

// Hlavní funkce
int main(int argc, char* argv[]) {
    if (!validateArguments(argc, argv)) {
        return 1;
    }

    std::string interfaceName, tcpPorts, udpPorts, host;
    int timeout = 5000;

    // Zpracování argumentu
    processArguments(argc, argv, interfaceName, tcpPorts, udpPorts, timeout, host);

    // Získání IP adres z hostname
    std::vector<std::string> ipAddresses = resolveHostToIP(host);
    if (ipAddresses.empty()) {
        std::cerr << "ERROR: No IP addresses found for hostname " << host << std::endl;
        return 1;
    }

    // Převod TCP portů na čísla
    std::vector<int> tcpPortNumbers = PortRangeParser::parsePortRanges(tcpPorts);
    for (const std::string &ip : ipAddresses) {
        for (int port : tcpPortNumbers) {
            TCPScanner::scanPort(ip, port);  // Místo localhost použijte IP
        }
    }

    // Převod UDP portů na čísla
    std::vector<int> udpPortNumbers = PortRangeParser::parsePortRanges(udpPorts);
    for (const std::string &ip : ipAddresses) {
        for (int port : udpPortNumbers) {
            UDPScanner::scanPort(ip, port);  // Místo localhost použijte IP
        }
    }

    return 0;
}
