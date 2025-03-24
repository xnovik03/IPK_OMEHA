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

// Funkce scanPort provádí skenování daného TCP portu na zadané IP adrese.
void TCPScanner::scanPort(const std::string &ip, int port) {
    // Získáme stav portu (OPEN, CLOSED, FILTERED) pomocí TCP connect techniky
    Packetsender::PortState state = Packetsender::sendTcpPacket(ip, port);
    // Výstup podle stavu portu
    if (state == Packetsender::OPEN) {
        ResultFormatter::formatTcpResult(ip, port, "open");
    } else if (state == Packetsender::CLOSED) {
        ResultFormatter::formatTcpResult(ip, port, "closed");
    } else {
        ResultFormatter::formatTcpResult(ip, port, "filtered");
    }
}
