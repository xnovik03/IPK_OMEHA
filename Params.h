#pragma once
#include <string>

class Params {
public:
    std::string interfaceName; // Síťové rozhraní
    std::string tcpPorts;      // TCP porty
    std::string udpPorts;      // UDP porty
    int timeout;               // Časový limit pro skenování portu
    std::string host;          // Hostname nebo IP adresa

    // Výchozí konstruktor
    Params() : timeout(5000) {}  // Timeout defaultně nastaven na 5000 ms

    // Metody pro nastavení hodnot
    void setInterfaceName(const std::string &iface) { interfaceName = iface; }
    void setTcpPorts(const std::string &ports) { tcpPorts = ports; }
    void setUdpPorts(const std::string &ports) { udpPorts = ports; }
    void setTimeout(int t) { timeout = t; }
    void setHost(const std::string &hostname) { host = hostname; }

    // Validace parametrů
    bool isValid() {
        return !interfaceName.empty() && !host.empty();
    }
};
