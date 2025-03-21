#include "Packetsender.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <netinet/ip.h>    // For IP header
#include <netinet/tcp.h>   // For TCP header (tcphdr and flags like TH_SYN)

bool Packetsender::sendTcpPacket(const std::string &ip, int port) {
    int sockfd;
    struct sockaddr_in dest;
    struct sockaddr_in6 dest6;

    if (ip.find(":") != std::string::npos) {  // Kontrola pro IPv6 adresy
        sockfd = socket(AF_INET6, SOCK_STREAM, 0);  // IPv6 socket
        if (sockfd == -1) {
            std::cerr << "Socket creation failed for IPv6" << std::endl;
            return false;
        }
        dest6.sin6_family = AF_INET6;
        dest6.sin6_port = htons(port);
        inet_pton(AF_INET6, ip.c_str(), &dest6.sin6_addr);
      
        if (connect(sockfd, (struct sockaddr *)&dest6, sizeof(dest6)) == 0) {
            close(sockfd);
            return true;
        }
    } else {  // IPv4
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            std::cerr << "Socket creation failed for IPv4" << std::endl;
            return false;
        }
        dest.sin_family = AF_INET;
        dest.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &dest.sin_addr);
        if (connect(sockfd, (struct sockaddr *)&dest, sizeof(dest)) == 0) {
            close(sockfd);
            return true;
        }
    }

    std::cerr << "Failed to connect to " << ip << ":" << port << std::endl;
    close(sockfd);
    return false;
}


bool Packetsender::sendUdpPacket(const std::string &ip, int port) {
    int sockfd;
    struct sockaddr_in dest;
    struct sockaddr_in6 dest6;

    if (ip.find(":") != std::string::npos) {  // IPv6
        sockfd = socket(AF_INET6, SOCK_DGRAM, 0);  // IPv6 socket
        if (sockfd == -1) {
            std::cerr << "Socket creation failed for IPv6" << std::endl;
            return false;
        }
        dest6.sin6_family = AF_INET6;
        dest6.sin6_port = htons(port);
        inet_pton(AF_INET6, ip.c_str(), &dest6.sin6_addr);
        // Odeslani UDP paketu na IPv6
        if (sendto(sockfd, "Test", 4, 0, (struct sockaddr *)&dest6, sizeof(dest6)) < 0) {
            std::cerr << "Error sending UDP packet to " << ip << ":" << port << std::endl;
            close(sockfd);
            return false;
        }
    } else {  // IPv4
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd == -1) {
            std::cerr << "Socket creation failed for IPv4" << std::endl;
            return false;
        }
        dest.sin_family = AF_INET;
        dest.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &dest.sin_addr);
        if (sendto(sockfd, "Test", 4, 0, (struct sockaddr *)&dest, sizeof(dest)) < 0) {
            std::cerr << "Error sending UDP packet to " << ip << ":" << port << std::endl;
            close(sockfd);
            return false;
        }
    }

    close(sockfd);
    return true;
}
