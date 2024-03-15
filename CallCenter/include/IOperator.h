#pragma once
#include <functional>
#include <memory>

#include "CDR.h"

class IOperator {
public:
    IOperator(){};

    virtual opID_t getId() const = 0;

    virtual void answer(const std::shared_ptr<CDR>& cdr, 
        std::function<void(opID_t)> callback, const time_point& endPoint) = 0;

    virtual ~IOperator() {};
private:
    IOperator(const IOperator& other) = delete;
    IOperator(IOperator&& other) = delete;
    IOperator& operator=(const IOperator& other) = delete;
    IOperator& operator=(IOperator&& other) = delete;
};