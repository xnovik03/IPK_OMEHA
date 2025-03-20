#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include "UDPscanner.h"
#include "Packetsender.h" 

void UDPScanner::scanPort(const std::string &ip, int port) {
    // Místo přímého odesílání UDP paketu voláme Packetsender
    bool result = Packetsender::sendUdpPacket(ip, port);  // Odeslání UDP paketu

    if (result) {
        std::cout << ip << " " << port << " udp open" << std::endl;
    } else {
        std::cout << ip << " " << port << " udp closed" << std::endl;
    }
}