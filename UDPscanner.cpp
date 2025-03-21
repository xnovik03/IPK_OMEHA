#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include "UDPscanner.h"
#include "Packetsender.h" 
#include "ResultFormatter.h"  


void UDPScanner::scanPort(const std::string &ip, int port) {
    struct addrinfo hints, *res;
    int sockfd;
    char msg[] = "Test";
    int result;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // AF_UNSPEC pro IPv4 i IPv6
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    // Volání getaddrinfo pro získání adresy
    result = getaddrinfo(ip.c_str(), std::to_string(port).c_str(), &hints, &res);
    if (result != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(result) << std::endl;
        return;
    }

    // Vytvoření socketu
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        std::cerr << "Socket creation failed." << std::endl;
        freeaddrinfo(res);
        return;
    }

    // Odeslání UDP paketu
    result = sendto(sockfd, msg, sizeof(msg), 0, res->ai_addr, res->ai_addrlen);
    if (result == -1) {
        std::cerr << "Error sending UDP packet to " << ip << ":" << port << std::endl;
    }

    // Čekání na odpověď
    char buffer[512];
    socklen_t fromlen = res->ai_addrlen;
    int bytesReceived = recvfrom(sockfd, buffer, sizeof(buffer), 0, res->ai_addr, &fromlen);

    if (bytesReceived == -1) {
        ResultFormatter::formatUdpResult(ip, port, "closed");
    } else {
        ResultFormatter::formatUdpResult(ip, port, "open");
    }

    close(sockfd);
    freeaddrinfo(res);  // Uvolníme alokovanou paměť
}