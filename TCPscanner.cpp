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

// Implementace metody scanPort
void TCPScanner::scanPort(const std::string &ip, int port) {
    // Místo implementace odesílání paketů voláme Packetsender
    bool result = Packetsender::sendTcpPacket(ip, port);  // Odeslání TCP SYN paketu

    if (result) {
        std::cout << ip << " " << port << " tcp open" << std::endl;
    } else {
        std::cout << ip << " " << port << " tcp closed" << std::endl;
    }
}
