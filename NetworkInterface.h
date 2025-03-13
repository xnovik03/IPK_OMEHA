#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include <string>
#include <vector>

class NetworkInterface {
public:
    
    static std::vector<std::string> getAvailableInterfaces();

   
    static void selectInterface(const std::vector<std::string>& interfaces);
};

#endif // NETWORKINTERFACE_H
