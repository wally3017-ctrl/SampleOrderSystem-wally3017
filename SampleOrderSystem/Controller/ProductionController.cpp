#include "ProductionController.h"

ProductionController::ProductionController(ProductionQueue& queue, ProductionView& view)
    : queue_(queue), view_(view)
{}

void ProductionController::Run() {
    auto current = queue_.Peek();

    std::vector<ProductionJob> waiting;
    if (!queue_.IsEmpty()) {
        auto all = queue_.GetQueue();
        if (all.size() > 1)
            waiting.assign(all.begin() + 1, all.end());
    }

    view_.ShowQueue(current, waiting);
}
