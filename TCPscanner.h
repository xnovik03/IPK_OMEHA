#ifndef TCPSCANNER_H
#define TCPSCANNER_H

#include <string>
#include <vector>

class TCPScanner {
public:
    // Funkce pro skenování jednotlivého portu
    static void scanPort(const std::string &ip, int port);
};

#endif // TCPSCANNER_H
