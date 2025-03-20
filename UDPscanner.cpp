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
#include "ResultFormatter.h"  


void UDPScanner::scanPort(const std::string &ip, int port)
{
    bool result = Packetsender::sendUdpPacket(ip, port);  // Odeslání UDP paketu

    if (result) {
        ResultFormatter::formatUdpResult(ip, port, "open");
    } else {
        ResultFormatter::formatUdpResult(ip, port, "closed");
    }
}