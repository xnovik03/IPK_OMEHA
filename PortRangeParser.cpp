#include "PortRangeParser.h"

// Funkce pro zpracování portových rozsahů
std::vector<int> PortRangeParser::parsePortRanges(const std::string& portRange) {
    std::vector<int> ports;
    std::stringstream ss(portRange);
    std::string token;

    while (getline(ss, token, ',')) {
        processRange(token, ports);  // Zpracování každého portového rozsahu nebo portu
    }

    return ports;
}

void PortRangeParser::processRange(const std::string& range, std::vector<int>& ports) {
    size_t dashPos = range.find('-');
    if (dashPos != std::string::npos) {
        // Pokud je nalezen '-', jedná se o rozsah
        int startPort = std::stoi(range.substr(0, dashPos));
        int endPort = std::stoi(range.substr(dashPos + 1));

        // Přidáme všechny porty v rozsahu
        for (int port = startPort; port <= endPort; ++port) {
            ports.push_back(port);
        }
    } else {
        // Pokud není '-', jedná se o jednotlivý port
        ports.push_back(std::stoi(range));
    }
}
