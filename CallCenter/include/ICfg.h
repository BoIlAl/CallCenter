#pragma once
#include <string>
#include <vector>

#include <types.h>

class ICfg {
public:
    ICfg(){};

    virtual std::vector<std::string> getNames() const = 0;
    virtual bool set(const json& values) = 0;

    virtual std::vector<size_t> getIndecesOfDefaultValues() const = 0;

    virtual ~ICfg() {};

    virtual std::string toStr() const = 0;
protected:
    static bool getUIntValue(const json& js, const std::string& name, uint16_t min, uint16_t max, uint16_t& res);
    static bool getUIntValue(const json& js, const std::string& name, uint64_t min, uint64_t max, uint64_t& res);
    static bool getBoolValue(const json& js, const std::string& name, bool& res);
private:
    ICfg(const ICfg& other) = delete;
    ICfg(ICfg&& other) = delete;
    ICfg& operator=(const ICfg& other) = delete;
    ICfg& operator=(ICfg&& other) = delete;
};