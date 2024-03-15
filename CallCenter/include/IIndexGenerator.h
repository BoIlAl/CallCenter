#pragma once
#include <cstdint>

class IIndexGenerator {
private:
    static constexpr uint64_t m_errorValue = 0;
public:
    IIndexGenerator(){};

    virtual void remove(uint64_t index) = 0;
    virtual uint64_t next() = 0;

    static uint64_t errorValue() { return m_errorValue; };

    virtual ~IIndexGenerator() {};
private:
    IIndexGenerator(const IIndexGenerator& other) = delete;
    IIndexGenerator(IIndexGenerator&& other) = delete;
    IIndexGenerator& operator=(const IIndexGenerator& other) = delete;
    IIndexGenerator& operator=(IIndexGenerator&& other) = delete;
};