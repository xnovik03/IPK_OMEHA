#pragma once
#include <string>
#include <netinet/in.h>

class Packetsender
{
public:
    // Metoda pro odeslání SYN paketu pro TCP skenování
    static bool sendTcpPacket(const std::string &ip, int port);

    // Metoda pro odeslání UDP paketu
    static bool sendUdpPacket(const std::string &ip, int port);
};
