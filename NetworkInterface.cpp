#include "NetworkInterface.h"
#include <iostream>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

// Vrátí seznam dostupných síťových rozhraní (IPv4)
std::vector<std::string> NetworkInterface::getAvailableInterfaces() {
    std::vector<std::string> interfaces;
    struct ifaddrs* ifap, * ifa;
    struct sockaddr_in* sa;
    char addr[INET_ADDRSTRLEN];
    // Získá seznam síťových rozhraní v systému
    if (getifaddrs(&ifap) == -1) {
        perror("getifaddrs");
        return interfaces;
    }
    // Projde všechna síťová rozhraní
    for (ifa = ifap; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr->sa_family == AF_INET) {  // IPv4
            sa = (struct sockaddr_in*)ifa->ifa_addr;
            inet_ntop(AF_INET, &(sa->sin_addr), addr, INET_ADDRSTRLEN);
            // Přidá jméno rozhraní do seznamu
            interfaces.push_back(ifa->ifa_name);
        }
    }
    // Uvolní paměť alokovanou funkcí getifaddrs
    freeifaddrs(ifap); 
    return interfaces;
}
// Nabídne uživateli seznam rozhraní a umožní mu vybrat jedno
void NetworkInterface::selectInterface(const std::vector<std::string>& interfaces) {
    std::cout << "Dostupna sitova rozhrani:" << std::endl;
    // Vypsání všech dostupných rozhraní s indexem
    for (size_t i = 0; i < interfaces.size(); ++i) {
        std::cout << i + 1 << ". " << interfaces[i] << std::endl;
    }

    int choice;
    std::cout << "Zvolte cislo rozhrani pro skenovani: ";
    std::cin >> choice;

    if (choice < 1 || choice > interfaces.size()) {
        std::cout << "Neplatna volba!" << std::endl;
        return;
    }

    std::cout << "Vybrane rozhrani: " << interfaces[choice - 1] << std::endl;
}
