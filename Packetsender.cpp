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
    // Kontrola, zda je IP adresa platná
    if (ip.empty() || inet_addr(ip.c_str()) == INADDR_NONE) {
        std::cerr << "Invalid IP address: " << ip << std::endl;
        return false;
    }

    // Vytvoření socketu pro TCP
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Socket creation failed for TCP" << std::endl;
        return false;
    }

    struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);
    dest.sin_addr.s_addr = inet_addr(ip.c_str());  // Použití zadané IP adresy

    // Pokus o připojení na TCP port
    if (connect(sockfd, (struct sockaddr*)&dest, sizeof(dest)) < 0) {
        std::cerr << "Connection failed to " << ip << ":" << port << std::endl;
        close(sockfd);
        return false;
    }

    std::cout << "TCP connection successful to " << ip << ":" << port << std::endl;
    close(sockfd);
    return true;
}


// Implementace funkce pro odeslání UDP paketu
bool Packetsender::sendUdpPacket(const std::string &ip, int port) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Socket creation failed for UDP" << std::endl;
        return false;
    }

    struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);
    dest.sin_addr.s_addr = inet_addr(ip.c_str());

    // Odeslání UDP paketu
    char message[] = "Test";
    if (sendto(sockfd, message, sizeof(message), 0, (struct sockaddr *)&dest, sizeof(dest)) < 0) {
        std::cerr << "Error sending UDP packet to " << ip << ":" << port << std::endl;
        close(sockfd);
        return false;
    }

    std::cout << "UDP packet sent to " << ip << ":" << port << std::endl;
    close(sockfd);
    return true;
}
