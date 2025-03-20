#include "Packetsender.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

// Metoda pro odeslání TCP SYN paketu
bool Packetsender::sendTcpPacket(const std::string &ip, int port)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Socket creation failed for TCP" << std::endl;
        return false;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip.c_str());

    // Pokus o připojení (bez dokončení 3-way handshake)
    int result = connect(sockfd, (struct sockaddr *)&server, sizeof(server));
    if (result == -1) {
        std::cerr << "TCP connection failed to " << ip << ":" << port << std::endl;
        close(sockfd);
        return false;
    }

    std::cout << "TCP packet sent to " << ip << ":" << port << std::endl;
    close(sockfd);
    return true;
}

// Metoda pro odeslání UDP paketu
bool Packetsender::sendUdpPacket(const std::string &ip, int port)
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        std::cerr << "Socket creation failed for UDP" << std::endl;
        return false;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip.c_str());

    // Odeslání UDP paketu
    char message[] = "Test UDP Packet";
    int result = sendto(sockfd, message, sizeof(message), 0, (struct sockaddr *)&server, sizeof(server));
    if (result == -1) {
        std::cerr << "UDP packet sending failed to " << ip << ":" << port << std::endl;
        close(sockfd);
        return false;
    }

    std::cout << "UDP packet sent to " << ip << ":" << port << std::endl;
    close(sockfd);
    return true;
}
