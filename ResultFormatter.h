#pragma once
#include <string>
#include <iostream>

class ResultFormatter
{
public:
    // Formátování výsledků pro TCP porty
    static void formatTcpResult(const std::string &ip, int port, const std::string &status);

    // Formátování výsledků pro UDP porty
    static void formatUdpResult(const std::string &ip, int port, const std::string &status);
};
