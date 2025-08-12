#include "Util.h"
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <functional>

namespace Util {

std::string trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

std::string nowTimestamp() {
    std::time_t t = std::time(nullptr);
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string hashPin(const std::string& pin, const std::string& salt) {
    // Demo-only hash: std::hash(salt + pin)
    std::hash<std::string> h;
    size_t v = h(salt + pin);
    std::ostringstream oss;
    oss << std::hex << v;
    return oss.str();
}

bool isDigits(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

} // namespace Util
