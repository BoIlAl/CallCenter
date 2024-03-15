#pragma once
#include <mutex>

#include "IOperatorManagerCfg.h"
#include "common.h"

class OperatorManagerCfg : public IOperatorManagerCfg {
private:
    static constexpr char m_rOperatorMinString[] = "ROperatorMin";
    static constexpr char m_rOperatorMaxString[] = "ROperatorMax";

    static const std::vector<std::string> m_valuesNames;

    static const constexpr ms_t m_rOperatorMinMin = 1000;
    static const constexpr ms_t m_rOperatorMinDefault = 30000;
    static const constexpr ms_t m_rOperatorMinMax = 3600000;

    static const constexpr ms_t m_rOperatorMaxMin = 1000;
    static const constexpr ms_t m_rOperatorMaxDefault = 60000;
    static const constexpr ms_t m_rOperatorMaxMax = 3600000;

    ms_t m_rOperatorMin;
    ms_t m_rOperatorMax;

    std::vector<size_t> m_indOfDefaultVals;

    mutable std::mutex m_mutex;

public:
    OperatorManagerCfg();

    virtual std::vector<std::string> getNames() const override;
    virtual bool set(const json& values) override;
    virtual std::vector<size_t> getIndecesOfDefaultValues() const override;

    virtual ms_t rOperatorMin() const override;
    virtual ms_t rOperatorMax() const override;

    virtual std::string toStr() const override;

    virtual ~OperatorManagerCfg() override {};
private:
    void setDefaultROperatorMin();
    void setDefaultROperatorMax();
    void setDefault();
};