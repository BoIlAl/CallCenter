#pragma once
#include <stdint.h>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <random>

#include "types.h"

inline bool isUInt(const std::string& str) {
    for(const char& ch : str) {
        if ('0' > ch || ch > '9') {
            return false;
        }
    }
    return true;
}

inline bool tryParseBool(const std::string& str, bool& res) {
    auto tr = "true";
    auto fls = "false";

    if (str.compare(tr) == 0) {
        res = true;
        return true;
    }

    if (str.compare(fls) == 0) {
        res = false;
        return true;
    }

    return false;
}

inline bool tryParseUInt(const std::string& str, uint16_t &res) {
    if (!isUInt(str)) {
        return false;
    }

    std::stringstream ss(str);

    if (!ss) {
        return false;
    }

    uint16_t tmp = res;
    if (!(ss >> res)) {
        res = tmp;
        return false;
    }

    return true;
}

inline bool tryParseUInt(const std::string& str, uint32_t &res) {
    if (!isUInt(str)) {
        return false;
    }

    std::stringstream ss(str);

    if (!ss) {
        return false;
    }

    uint32_t tmp = res;
    if (!(ss >> res)) {
        res = tmp;
        return false;
    }

    return true;
}

inline bool tryParseUInt(const std::string& str, uint64_t &res) {
    if (!isUInt(str)) {
        return false;
    }

    std::stringstream ss(str);

    if (!ss) {
        return false;
    }

    uint64_t tmp = res;
    if (!(ss >> res)) {
        res = tmp;
        return false;
    }

    return true;
}

inline std::string tpToString(time_point tp) {

    using namespace std::chrono;

    auto ms = duration_cast<milliseconds>(tp.time_since_epoch()) % 1000;

    auto timer = system_clock::to_time_t(tp);

    std::tm* bt = std::localtime(&timer);

    if (bt == nullptr) {
        return std::string();
    }

    std::ostringstream oss;

    oss << std::put_time(bt, "%F %T");
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

    if (!oss) {
        return std::string();
    }

    return oss.str();
}

inline time_point getPointAfterRndInterval(ms_t min, ms_t max) {
    if (min > max) {
        return time_point::min();
    } 
    else if (min == max) {
        return std::chrono::system_clock::now() + std::chrono::milliseconds(min);
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(min, max);
    return std::chrono::system_clock::now() + std::chrono::milliseconds(dist(gen));
}

inline auto tm(std::string string) {
    return TM(string.c_str());
}