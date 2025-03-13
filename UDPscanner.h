#ifndef UDPSCANNER_H
#define UDPSCANNER_H

#include <string>
#include <vector>

class UDPScanner {
public:
    static void scanPort(const std::string &ip, int port);  // Deklarace metody
};

#endif // UDPSCANNER_H
