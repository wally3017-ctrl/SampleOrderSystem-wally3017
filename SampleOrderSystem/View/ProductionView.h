#pragma once
#include "ConsoleView.h"
#include "../Model/ProductionJob.h"
#include <optional>
#include <vector>

class ProductionView {
    ConsoleView& console_;
public:
    explicit ProductionView(ConsoleView& console);
    void ShowQueue(const std::optional<ProductionJob>& current,
                   const std::vector<ProductionJob>&    waiting);
};
