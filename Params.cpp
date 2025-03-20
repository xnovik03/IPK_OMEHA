#include "Params.h"
#include <iostream>
#include <cstdlib>

// Ověříme, že všechny parametry byly správně nastaveny
bool Params::isValid() {
    if (interfaceName.empty()) {
        std::cerr << "ERROR: Network interface is not specified." << std::endl;
        return false;
    }
    if (host.empty()) {
        std::cerr << "ERROR: Hostname or IP address is not specified." << std::endl;
        return false;
    }
    return true;
}
