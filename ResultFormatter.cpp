#include "ResultFormatter.h"
#include <iostream>

// Implementace formátování výsledků pro TCP
void ResultFormatter::formatTcpResult(const std::string &ip, int port, const std::string &status)
{
    // Výstup formátovaný pro TCP
    std::cout << ip << " " << port << " tcp " << status << std::endl;
}

// Implementace formátování výsledků pro UDP
void ResultFormatter::formatUdpResult(const std::string &ip, int port, const std::string &status)
{
    // Výstup formátovaný pro UDP
    std::cout << ip << " " << port << " udp " << status << std::endl;
}
