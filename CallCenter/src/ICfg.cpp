#include "ICfg.h"

bool ICfg::getUIntValue(const json& js, const std::string& name, uint16_t min, uint16_t max, uint16_t& res) {
    try {
        auto value = js.at(name);

        if (value.is_number_unsigned() && value >= min && value <= max) {
            res = value;
        }
        else {
            return false;
        }
    }
    catch (json::type_error te) {
        return false;
    }
    catch (json::out_of_range oor) {
        return false;
    }

    return true;
}

bool ICfg::getUIntValue(const json& js, const std::string& name, uint64_t min, uint64_t max, uint64_t& res) {
    try {
        auto value = js.at(name);

        if (value.is_number_unsigned() && value >= min && value <= max) {
            res = value;
        }
        else {
            return false;
        }
    }
    catch (json::type_error te) {
        return false;
    }
    catch (json::out_of_range oor) {
        return false;
    }

    return true;
}

bool ICfg::getBoolValue(const json& js, const std::string& name, bool& res) {
    try {
        auto value = js.at(name);

        if (value.is_boolean()) {
            res = value;
        } 
        else {
            return false;
        }

        res = value;
    }
    catch (json::type_error te) {
        return false;
    }
    catch (json::out_of_range oor) {
        return false;
    }

    return true;
}