#include "ProductionController.h"

ProductionController::ProductionController(IProductionQueue&    queue,
                                           IRepository<Order>&  orderRepo,
                                           IRepository<Sample>& sampleRepo,
                                           ProductionView&      view)
    : queue_(queue), orderRepo_(orderRepo), sampleRepo_(sampleRepo), view_(view)
{}

void ProductionController::Run() {
    auto all     = queue_.GetQueue();
    auto current = all.empty()
        ? std::optional<ProductionJob>{}
        : std::optional<ProductionJob>(all.front());
    std::vector<ProductionJob> waiting(
        all.size() > 1 ? all.begin() + 1 : all.end(), all.end());

    view_.ShowQueue(current, waiting);
}
