#pragma once
#include <mutex>

#include "ICDRHandlerCfg.h"
#include "common.h"

class CDRHandlerCfg : public ICDRHandlerCfg {
private:
    static constexpr char m_rQueueMinString[] = "RQueueMin";
    static constexpr char m_rQueueMaxString[] = "RQueueMax";
    static constexpr char m_isUpdateDublicatesString[] = "IsUpdateDublicates";
    static constexpr char m_queueLengthString[] = "QueueLength";

    static const std::vector<std::string> m_valuesNames;

    static const constexpr uint16_t m_queueLengthMax = 1000;
    static const constexpr uint16_t m_queueLengthDefault = 50;
    static const constexpr uint16_t m_queueLengthMin = 1;

    static const constexpr ms_t m_rQueueMinMin = 1000;
    static const constexpr ms_t m_rQueueMinDefault = 30000;
    static const constexpr ms_t m_rQueueMinMax = 3600000;

    static const constexpr ms_t m_rQueueMaxMin = 1000;
    static const constexpr ms_t m_rQueueMaxDefault = 60000;
    static const constexpr ms_t m_rQueueMaxMax = 3600000;

    static const constexpr ms_t m_isUpdateDublicatesDefault = true;

    ms_t m_rQueueMin;
    ms_t m_rQueueMax;
    bool m_isUpdateDublicates;
    uint16_t m_queueLength;

    std::vector<size_t> m_indOfDefaultVals;

    mutable std::mutex m_mutex;
public:
    CDRHandlerCfg();

    virtual std::vector<std::string> getNames() const override;
    virtual bool set(const json& values) override;
    virtual std::vector<size_t> getIndecesOfDefaultValues() const override;

    virtual ms_t rQueueMin() const override;
    virtual ms_t rQueueMax() const override;
    
    virtual bool isUpdateDublicates() const override;
    virtual uint16_t queueLength() const override;

    virtual std::string toStr() const override;

    virtual ~CDRHandlerCfg() override {};
private:
    void setDefaultRQueueMin();
    void setDefaultRQueueMax();
    void setDefaultIsUpdateDublicates();
    void setDefaultQueueLength();
    void setDefault();
};