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
#include "ResultFormatter.h" 

// Implementace metody scanPort
void TCPScanner::scanPort(const std::string &ip, int port)
{
    bool result = Packetsender::sendTcpPacket(ip, port);  // Odeslání TCP SYN paketu

    if (result) {
        ResultFormatter::formatTcpResult(ip, port, "open");
    } else {
        ResultFormatter::formatTcpResult(ip, port, "closed");
    }
}
