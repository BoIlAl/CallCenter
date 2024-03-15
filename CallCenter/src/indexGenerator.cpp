#include "indexGenerator.h"

IndexGenerator::IndexGenerator()
    : m_currValue(1)
{}
    
uint64_t IndexGenerator::next() {
    uint64_t firstVal = m_currValue;

    while(m_existing.find(m_currValue) != m_existing.end()) {
        m_currValue++;
        if (m_currValue == IIndexGenerator::errorValue()) {
            m_currValue++;
        }

        if (firstVal == m_currValue) {
            return IIndexGenerator::errorValue();
        }
    }

    m_existing.insert(m_currValue);
    return m_currValue;
}

void IndexGenerator::remove(uint64_t index) {
    m_existing.erase(index);
}