#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include "UDPscanner.h"

void UDPScanner::scanPort(const std::string &ip, int port) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        std::cout << "Socket creation failed." << std::endl;
        return;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip.c_str());

    // Odeslat UDP paket na port
    char msg[] = "Test";
    int result = sendto(sockfd, msg, sizeof(msg), 0, (struct sockaddr *)&server, sizeof(server));
    if (result == -1) {
        std::cout << "Error sending UDP packet." << std::endl;
    }

    // Čekání na odpověď
    struct sockaddr_in from;
    socklen_t fromlen = sizeof(from);
    char buffer[512];
    int bytesReceived = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&from, &fromlen);

    if (bytesReceived == -1) {
        std::cout << ip << " " << port << " udp closed" << std::endl;
    } else {
        std::cout << ip << " " << port << " udp open" << std::endl;
    }

    close(sockfd);
}
