#include "CDRHandlerCfg.h"

constexpr char CDRHandlerCfg::m_rQueueMinString[];
constexpr char CDRHandlerCfg::m_rQueueMaxString[];
constexpr char CDRHandlerCfg::m_isUpdateDublicatesString[];
constexpr char CDRHandlerCfg::m_queueLengthString[];

const std::vector<std::string> CDRHandlerCfg::m_valuesNames({
    m_rQueueMinString,
    m_rQueueMaxString,
    m_isUpdateDublicatesString,
    m_queueLengthString
});

CDRHandlerCfg::CDRHandlerCfg() {
    setDefault();
}

std::vector<std::string> CDRHandlerCfg::getNames() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_valuesNames;
}

bool CDRHandlerCfg::set(const json& values) {
    std::lock_guard<std::mutex> lock(m_mutex);

    m_indOfDefaultVals.clear();
    bool isNoDefault = true;

    bool isOk = ICfg::getUIntValue(values, m_valuesNames[0], m_rQueueMinMin, m_rQueueMinMax, m_rQueueMin);
    if (!isOk) {
        setDefaultRQueueMin();
        isNoDefault = false;
    }

    isOk = ICfg::getUIntValue(values, m_valuesNames[1], m_rQueueMaxMin, m_rQueueMaxMax, m_rQueueMax);
    if (!isOk) {
        setDefaultRQueueMax();
        isNoDefault = false;
    }

    if (m_rQueueMin > m_rQueueMax) {
        m_indOfDefaultVals.clear();
        setDefaultRQueueMax();
        setDefaultRQueueMin();
        isNoDefault = false;
    }

    isOk = ICfg::getBoolValue(values, m_valuesNames[2], m_isUpdateDublicates);
    if (!isOk) {
        setDefaultIsUpdateDublicates();
        isNoDefault = false;
    }

    isOk = ICfg::getUIntValue(values, m_valuesNames[3], m_queueLengthMin, m_queueLengthMax, m_queueLength);
    if (!isOk) {
        setDefaultQueueLength();
        isNoDefault = false;
    }

    return isNoDefault;
}

ms_t CDRHandlerCfg::rQueueMin() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_rQueueMin;
}

ms_t CDRHandlerCfg::rQueueMax() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_rQueueMax;
}

bool CDRHandlerCfg::isUpdateDublicates() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_isUpdateDublicates;
}

uint16_t CDRHandlerCfg::queueLength() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_queueLength;
}

std::string CDRHandlerCfg::toStr() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::stringstream ss;

    ss << m_rQueueMinString << ": " << m_rQueueMin << std::endl
       << m_rQueueMaxString << ": " << m_rQueueMax << std::endl
       << m_isUpdateDublicatesString << ": " << m_isUpdateDublicates << std::endl
       << m_queueLengthString << ": " << m_queueLength << std::endl;
    return ss.str();
}


std::vector<size_t> CDRHandlerCfg::getIndecesOfDefaultValues() const {
    return m_indOfDefaultVals;
}

void CDRHandlerCfg::setDefaultRQueueMin() {
    m_rQueueMin = m_rQueueMinDefault;
    m_indOfDefaultVals.push_back(0);
}

void CDRHandlerCfg::setDefaultRQueueMax() {
    m_rQueueMax = m_rQueueMaxDefault;
    m_indOfDefaultVals.push_back(1);
}

void CDRHandlerCfg::setDefaultIsUpdateDublicates() {
    m_isUpdateDublicates = m_isUpdateDublicatesDefault;
    m_indOfDefaultVals.push_back(2);
}

void CDRHandlerCfg::setDefaultQueueLength() {
    m_queueLength = m_queueLengthDefault;
    m_indOfDefaultVals.push_back(3);
}

void CDRHandlerCfg::setDefault() {
    setDefaultRQueueMin();
    setDefaultRQueueMax();
    setDefaultIsUpdateDublicates();
    setDefaultQueueLength();
}