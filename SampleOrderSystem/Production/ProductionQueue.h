#pragma once
#include "IProductionQueue.h"
#include <deque>
#include <string>

class ProductionQueue : public IProductionQueue {
    std::deque<ProductionJob> queue_;
    std::string               filename_;

    void SaveToFile() const;
    void LoadFromFile();

public:
    explicit ProductionQueue(const std::string& filename = "");

    void Enqueue(const ProductionJob& job) override;
    std::optional<ProductionJob> Dequeue() override;
    std::optional<ProductionJob> Peek() const override;
    std::vector<ProductionJob>   GetQueue() const override;

    bool        IsEmpty() const;
    std::size_t Size()    const;
};
