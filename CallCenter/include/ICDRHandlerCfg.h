#pragma once

#include "ICfg.h"

class ICDRHandlerCfg : public ICfg {
public:
    ICDRHandlerCfg(){};

    virtual ms_t rQueueMin() const = 0;
    virtual ms_t rQueueMax() const = 0;

    virtual bool isUpdateDublicates() const = 0;
    virtual uint16_t queueLength() const = 0;

    virtual ~ICDRHandlerCfg() override {};
private:
    ICDRHandlerCfg(const ICDRHandlerCfg& other) = delete;
    ICDRHandlerCfg(ICDRHandlerCfg&& other) = delete;
    ICDRHandlerCfg& operator=(const ICDRHandlerCfg& other) = delete;
    ICDRHandlerCfg& operator=(ICDRHandlerCfg&& other) = delete;
};