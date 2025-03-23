#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include "TCPscanner.h"
#include "Packetsender.h"
#include "ResultFormatter.h" 

void TCPScanner::scanPort(const std::string &ip, int port) {
    Packetsender::PortState state = Packetsender::sendTcpPacket(ip, port);

    if (state == Packetsender::OPEN) {
        ResultFormatter::formatTcpResult(ip, port, "open");
    } else if (state == Packetsender::CLOSED) {
        ResultFormatter::formatTcpResult(ip, port, "closed");
    } else {
        ResultFormatter::formatTcpResult(ip, port, "filtered");
    }
}
