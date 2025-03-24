#include "NetworkInterface.h"
#include <iostream>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

// Vrátí seznam dostupných síťových rozhraní 
std::vector<std::string> NetworkInterface::getAvailableInterfaces() {
    std::set<std::string> uniqueInterfaces;  // Set pro uložení unikátních názvů rozhraní
    struct ifaddrs* ifap, *ifa;

    // Získání seznamu všech síťových rozhraní
    if (getifaddrs(&ifap) == -1) {
        perror("getifaddrs");
        return {};  // Pokud dojde k chybě, vracíme prázdný vektor
    }

    // Iterace přes všechna rozhraní
    for (ifa = ifap; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == nullptr) continue;  // Přeskočíme, pokud nemá adresu

        int family = ifa->ifa_addr->sa_family;
        // Zajímá nás pouze IPv4 (AF_INET) nebo IPv6 (AF_INET6)
        if (family == AF_INET || family == AF_INET6) {
            uniqueInterfaces.insert(ifa->ifa_name);  // Přidáme název rozhraní do množiny
        }
    }

    // Uvolníme paměť alokovanou funkcí getifaddrs
    freeifaddrs(ifap);

    // Převedeme množinu na vektor a vrátíme ji
    return std::vector<std::string>(uniqueInterfaces.begin(), uniqueInterfaces.end());
}
