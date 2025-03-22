#pragma once
#include <string>

class Packetsender {
public:
    // Přidejte enum pro stav portu
    enum PortState {
        OPEN,
        CLOSED
    
    };

    // Deklarace metod
    static bool sendTcpPacket(const std::string &ip, int port);
    static unsigned short checksum(unsigned short *buf, int len);
    static PortState sendUdpPacket(const std::string &ip, int port);
};
