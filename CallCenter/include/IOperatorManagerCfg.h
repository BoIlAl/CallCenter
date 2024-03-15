#pragma once

#include "ICfg.h"

class IOperatorManagerCfg : public ICfg {
public:
    IOperatorManagerCfg(){};

    virtual ms_t rOperatorMin() const = 0;
    virtual ms_t rOperatorMax() const = 0;
    
    virtual ~IOperatorManagerCfg() override {};
private:
    IOperatorManagerCfg(const IOperatorManagerCfg& other) = delete;
    IOperatorManagerCfg(IOperatorManagerCfg&& other) = delete;
    IOperatorManagerCfg& operator=(const IOperatorManagerCfg& other) = delete;
    IOperatorManagerCfg& operator=(IOperatorManagerCfg&& other) = delete;
};