#include "operatorManagerCfg.h"

constexpr char OperatorManagerCfg::m_rOperatorMinString[];
constexpr char OperatorManagerCfg::m_rOperatorMaxString[];

const std::vector<std::string> OperatorManagerCfg::m_valuesNames({
    m_rOperatorMinString,
    m_rOperatorMaxString
});

OperatorManagerCfg::OperatorManagerCfg() {
    setDefault();
}

std::vector<std::string> OperatorManagerCfg::getNames() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_valuesNames;
}

bool OperatorManagerCfg::set(const json& values) {
    std::lock_guard<std::mutex> lock(m_mutex);

    m_indOfDefaultVals.clear();
    bool isNoDefault = true;

    bool isOk = ICfg::getUIntValue(values, m_valuesNames[0], m_rOperatorMinMin, m_rOperatorMinMax, m_rOperatorMin);
    if (!isOk) {
        setDefaultROperatorMin();
        isNoDefault = false;
    }

    isOk = ICfg::getUIntValue(values, m_valuesNames[1], m_rOperatorMaxMin, m_rOperatorMaxMax, m_rOperatorMax);
    if (!isOk) {
        setDefaultROperatorMax();
        isNoDefault = false;
    }

    if (m_rOperatorMin > m_rOperatorMax) {
        m_indOfDefaultVals.clear();
        setDefaultROperatorMax();
        setDefaultROperatorMin();
        isNoDefault = false;
    }

    return isNoDefault;
}
ms_t OperatorManagerCfg::rOperatorMin() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_rOperatorMin;
}

ms_t OperatorManagerCfg::rOperatorMax() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_rOperatorMax;
}

std::vector<size_t> OperatorManagerCfg::getIndecesOfDefaultValues() const {
    return m_indOfDefaultVals;
}

std::string OperatorManagerCfg::toStr() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::stringstream ss;

    ss << m_rOperatorMinString << ": " << m_rOperatorMin << std::endl
       << m_rOperatorMaxString << ": " << m_rOperatorMax << std::endl;
    return ss.str();
}

void OperatorManagerCfg::setDefaultROperatorMin() {
    m_rOperatorMin = m_rOperatorMinDefault;
    m_indOfDefaultVals.push_back(0);
}

void OperatorManagerCfg::setDefaultROperatorMax() {
    m_rOperatorMax = m_rOperatorMaxDefault;
    m_indOfDefaultVals.push_back(1);
}

void OperatorManagerCfg::setDefault() {
    setDefaultROperatorMin();
    setDefaultROperatorMax();
}