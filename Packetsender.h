#pragma once
#include <string>

lass Packetsender {
public:
    // Přidejte enum pro stav portu
    enum PortState {
        OPEN,
        CLOSED,
        FILTERED
    };

    // Deklarace metod
    static Packetsender::PortState sendTcpPacket(const std::string &ip, int port);
    static unsigned short checksum(unsigned short *buf, int len);
    static PortState sendUdpPacket(const std::string &ip, int port);
};