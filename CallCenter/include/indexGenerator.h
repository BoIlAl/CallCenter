#pragma once
#include <unordered_set>

#include "IIndexGenerator.h"

class IndexGenerator : public IIndexGenerator {
private:
    uint64_t m_currValue;

    std::unordered_set<uint64_t> m_existing;
public:
    IndexGenerator();
    
    virtual uint64_t next() override;
    virtual void remove(uint64_t index) override;

    ~IndexGenerator() override {};
};