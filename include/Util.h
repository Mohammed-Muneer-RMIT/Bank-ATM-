#pragma once
#include <string>

namespace Util {
    std::string trim(const std::string& s);
    std::string nowTimestamp();
    std::string hashPin(const std::string& pin, const std::string& salt);
    bool isDigits(const std::string& s);
}
