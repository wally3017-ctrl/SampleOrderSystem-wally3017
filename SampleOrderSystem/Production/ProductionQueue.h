#pragma once
#include "IProductionQueue.h"
#include <vector>

class ProductionQueue : public IProductionQueue {
    std::vector<Order> queue_;
public:
    void Enqueue(const Order& order) override { queue_.push_back(order); }
    const std::vector<Order>& GetQueue() const { return queue_; }
    std::size_t Size() const { return queue_.size(); }
};
