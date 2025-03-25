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
#include "Params.h"

std::vector<std::string> resolveHostToIP(const std::string &host);

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
        std::cerr << "ERROR: No IP addresses found for hostname " << host << std::endl;

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


// Zpracování argumentů z příkazové řádky
void processArguments(int argc, char* argv[], Params &params) {
    bool interfaceProvided = false;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
       

         if (arg == "-i" || arg == "--interface") {
            if (i + 1 < argc) {
                params.setInterfaceName(argv[i + 1]);
                interfaceProvided = true;
                i++;
            } else {
                std::cout << "Available interfaces:" << std::endl;
                
                // Získání seznamu dostupných síťových rozhraní
                NetworkInterface netInterface;
                std::vector<std::string> interfaces = netInterface.getAvailableInterfaces();
                for (const auto& iface : interfaces) {
                    std::cout << iface << std::endl;
                }
                return;  
            }
        } 
        else if (arg == "--pt" || arg == "-t") {
            if (i + 1 < argc) {
                params.setTcpPorts(argv[i + 1]);
                i++;
            }
        }
        else if (arg == "--pu" || arg == "-u") {
            if (i + 1 < argc) {
                params.setUdpPorts(argv[i + 1]);
                i++;
            }
        }
        else if (arg == "--wait" || arg == "-w") {
            if (i + 1 < argc) {
                params.setTimeout(std::stoi(argv[i + 1]));
                i++;
            }
        }
        else {
            // Hostname or IP address 
            params.setHost(argv[i]);
        }
    }
    if (!interfaceProvided) {
        std::cout << "ERROR: Network interface not provided" << std::endl;
    }
}

// Hlavní funkce
int main(int argc, char* argv[]) {
    Params params;  // Vytvoření instance Params

   // Pokud je --help nebo -h, vypíšeme nápovědu a ukončíme program
    if (argc == 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")) {
        std::cout << "Usage: ./ipk-l4-scan [-i interface | --interface interface] "
                  << "[--pu port-ranges | --pt port-ranges | -u port-ranges | -t port-ranges] "
                  << "[-w timeout] [hostname | ip-address]" << std::endl;
        std::cout << "-i, --interface        Network interface to scan through" << std::endl;
        std::cout << "-t, --pt               TCP port ranges to scan (e.g., --pt 22,23,24)" << std::endl;
        std::cout << "-u, --pu               UDP port ranges to scan" << std::endl;
        std::cout << "-w, --wait             Timeout for each port scan in milliseconds" << std::endl;
        std::cout << "hostname               Hostname or IP address to scan" << std::endl;
        return 0;  // Ukončí program po zobrazení nápovědy
    }

    // Zpracování argumentů
    processArguments(argc, argv, params);

    // Validace parametrů
    if (!params.isValid()) {
        return 1;
    }

    // Získání IP adres z hostname
    std::vector<std::string> ipAddresses = resolveHostToIP(params.host);
    if (ipAddresses.empty()) {
        std::cerr << "ERROR: No IP addresses found for hostname " << params.host << std::endl;
        return 1;
    }

    // Převod TCP portů na čísla
    std::vector<int> tcpPortNumbers = PortRangeParser::parsePortRanges(params.tcpPorts);
    for (const std::string &ip : ipAddresses) {
        for (int port : tcpPortNumbers) {
            TCPScanner::scanPort(ip, port);  
        }
    }

    // Převod UDP portů na čísla
    std::vector<int> udpPortNumbers = PortRangeParser::parsePortRanges(params.udpPorts);
    for (const std::string &ip : ipAddresses) {
        for (int port : udpPortNumbers) {
            UDPScanner::scanPort(ip, port); 
        }
    }

    return 0;
}
