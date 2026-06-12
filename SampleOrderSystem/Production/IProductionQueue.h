#pragma once
#include "../Model/Order.h"

class IProductionQueue {
public:
    virtual ~IProductionQueue() = default;
    virtual void Enqueue(const Order& order) = 0;
};
