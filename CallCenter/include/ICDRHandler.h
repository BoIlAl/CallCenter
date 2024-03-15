#pragma once
#include <sstream>

#include "CDR.h"

class ICDRHandler {
public:
    ICDRHandler(){};

    virtual ReleaseCause handleNumber(uint64_t number) = 0;
    virtual std::shared_ptr<CDR> next() = 0;
    virtual void handleFilled(const std::shared_ptr<CDR>& cdr) = 0;

    virtual ~ICDRHandler() {};
private:
    ICDRHandler(const ICDRHandler& other) = delete;
    ICDRHandler(ICDRHandler&& other) = delete;
    ICDRHandler& operator=(const ICDRHandler& other) = delete;
    ICDRHandler& operator=(ICDRHandler&& other) = delete;
};