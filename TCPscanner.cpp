#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include "TCPscanner.h"
#include "Packetsender.h"
#include "ResultFormatter.h" 

// Implementace metody scanPort
void TCPScanner::scanPort(const std::string &ip, int port) {
    struct addrinfo hints, *res;
    int sockfd, result;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;  // Pro IPv4 i IPv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

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

    // Pokus o připojení k portu
    result = connect(sockfd, res->ai_addr, res->ai_addrlen);

    // Pokud dojde k chybě, port je zavřený
    if (result == -1) {
        ResultFormatter::formatTcpResult(ip, port, "closed");
    } else {
        ResultFormatter::formatTcpResult(ip, port, "open");
    }

    close(sockfd);
    freeaddrinfo(res);  // Uvolníme alokovanou paměť
}
