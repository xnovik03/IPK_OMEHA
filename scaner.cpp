#include "scaner.h"
#include <iostream>
#include <string>
#include "NetworkInterface.h"
#include "PortRangeParser.h"
#include "TCPscanner.h"
#include "UDPscanner.h"

using namespace std;



    // Zajištění, že byly zadány všechny parametry
    if (interface.empty() || tcpPorts.empty() || udpPorts.empty() || host.empty()) {
        cerr << "ERROR: Missing required arguments." << endl;
        cerr << "Usage: ./ipk-l4-scan -i <interface> -t <port> -u <port> -w <timeout> <hostname>" << endl;
        return;
    }

   

    // Získání IP adres pro zadaný host
    vector<string> ipAddresses = resolveHostToIP(host);
    if (ipAddresses.empty()) {
        cerr << "ERROR: No IP addresses found for hostname " << host << endl;
        return;
    }

    // Skenování TCP portů
    vector<int> tcpPortNumbers = PortRangeParser::parsePortRanges(tcpPorts);
    for (const string &ip : ipAddresses) {
        for (int port : tcpPortNumbers) {
            TCPScanner::scanPort(ip, port);  // Pro každou IP adresu provádí skenování
        }
    }

    // Skenování UDP portů
    vector<int> udpPortNumbers = PortRangeParser::parsePortRanges(udpPorts);
    for (const string &ip : ipAddresses) {
        for (int port : udpPortNumbers) {
            UDPScanner::scanPort(ip, port);  // Pro každou IP adresu provádí skenování
        }
    }
}
