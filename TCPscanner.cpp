#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include "TCPscanner.h"

// Implementace metody scanPort
void TCPScanner::scanPort(const std::string &ip, int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cout << "Socket creation failed." << std::endl;
        return;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip.c_str());

    // Pokus o připojení na port
    int result = connect(sockfd, (struct sockaddr *)&server, sizeof(server));
    
    if (result == -1) {
        std::cout << ip << " " << port << " tcp closed" << std::endl;
    } else {
        std::cout << ip << " " << port << " tcp open" << std::endl;
    }

    close(sockfd);
}
