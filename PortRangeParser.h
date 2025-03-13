#ifndef PORTRANGEPARSER_H
#define PORTRANGEPARSER_H

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

class PortRangeParser {
public:
    // Metoda pro zpracování portových rozsahů
    static std::vector<int> parsePortRanges(const std::string& portRange);

private:
    // Pomocná funkce pro zpracování jednotlivých portů a rozsahů
    static void processRange(const std::string& range, std::vector<int>& ports);
};

#endif // PORTRANGEPARSER_H
