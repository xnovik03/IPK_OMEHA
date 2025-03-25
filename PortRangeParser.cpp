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

// Přidá do vektoru porty ze zadaného rozsahu nebo jednoho portu ve stringu.
void PortRangeParser::processRange(const std::string& range, std::vector<int>& ports) {
    size_t dashPos = range.find('-');
    try {
        if (dashPos != std::string::npos) {
            // Pokud je nalezen '-', jedná se o rozsah
            int startPort = std::stoi(range.substr(0, dashPos));
            int endPort = std::stoi(range.substr(dashPos + 1));

            if (startPort <= 0 || endPort > 65535 || startPort > endPort) {
                std::cerr << "ERROR: Invalid port range: " << range << std::endl;
                exit(1);
            }

            // Přidáme všechny porty v rozsahu
            for (int port = startPort; port <= endPort; ++port) {
                ports.push_back(port);
            }
        } else {
            // Pokud není '-', jedná se o jednotlivý port
            int port = std::stoi(range);
            if (port <= 0 || port > 65535) {
                std::cerr << "ERROR: Invalid port number: " << port << std::endl;
                exit(1);
            }
            ports.push_back(port);
        }
    } catch (const std::invalid_argument&) {
        std::cerr << "ERROR: Invalid port format: " << range << std::endl;
        exit(1);
    } catch (const std::out_of_range&) {
        std::cerr << "ERROR: Port number out of range: " << range << std::endl;
        exit(1);
    }
}