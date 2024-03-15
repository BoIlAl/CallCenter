#pragma once
#include <thread>
#include <mutex>

#include "IOperator.h"

class Operator : public IOperator {
private:
    opID_t m_id;

    mutable std::mutex m_mutex;
public:
    Operator(opID_t id);

    virtual opID_t getId() const override;

    virtual void answer(const std::shared_ptr<CDR>& cdr, 
        std::function<void(opID_t)> callback, const time_point& endPoint) override;

    virtual ~Operator() override {};
};