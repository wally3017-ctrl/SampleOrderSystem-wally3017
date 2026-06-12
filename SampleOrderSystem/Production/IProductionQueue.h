#pragma once
#include "../Model/ProductionJob.h"
#include <optional>
#include <vector>

class IProductionQueue {
public:
    virtual ~IProductionQueue() = default;
    virtual void Enqueue(const ProductionJob& job) = 0;
    virtual std::optional<ProductionJob> Dequeue() = 0;
    virtual std::optional<ProductionJob> Peek() const = 0;
    virtual std::vector<ProductionJob>   GetQueue() const = 0;
};
