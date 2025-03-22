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
    Packetsender::PortState state = Packetsender::sendUdpPacket(ip, port);

    if (state == Packetsender::OPEN) {
        ResultFormatter::formatUdpResult(ip, port, "open");
    } else {  // CLOSED
        ResultFormatter::formatUdpResult(ip, port, "closed");
    }
}