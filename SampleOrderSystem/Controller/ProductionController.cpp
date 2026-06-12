#include "ProductionController.h"
#include "../Utility/Timestamp.h"
#include <stdexcept>

ProductionController::ProductionController(IProductionQueue&    queue,
                                           IRepository<Order>&  orderRepo,
                                           IRepository<Sample>& sampleRepo,
                                           ProductionView&      view)
    : queue_(queue), orderRepo_(orderRepo), sampleRepo_(sampleRepo), view_(view)
{}

void ProductionController::Complete() {
    auto jobOpt = queue_.Dequeue();
    if (!jobOpt.has_value()) return;

    const ProductionJob& job = jobOpt.value();

    auto orderOpt = orderRepo_.Load(job.GetOrderId());
    if (orderOpt.has_value()) {
        Order order = orderOpt.value();
        order.SetStatus(OrderStatus::CONFIRMED);
        orderRepo_.Update(order);
    }

    auto sampleOpt = sampleRepo_.Load(job.GetSampleId());
    if (sampleOpt.has_value()) {
        Sample sample = sampleOpt.value();
        sample.SetStock(sample.GetStock() + job.GetActualQty());
        sampleRepo_.Update(sample);
    }
}

void ProductionController::AutoComplete() {
    while (true) {
        auto opt = queue_.Peek();
        if (!opt.has_value()) break;
        if (!Timestamp::IsElapsed(opt.value().GetEnqueuedAt(), opt.value().GetTotalTime())) break;
        ProductionJob job = opt.value();
        Complete();
        view_.ShowAutoCompleted(job);
    }
}

void ProductionController::Run() {
    AutoComplete();
    auto all     = queue_.GetQueue();
    auto current = all.empty()
        ? std::optional<ProductionJob>{}
        : std::optional<ProductionJob>(all.front());
    std::vector<ProductionJob> waiting(
        all.size() > 1 ? all.begin() + 1 : all.end(), all.end());
    view_.ShowQueue(current, waiting);
    view_.ShowBackMenu();
    view_.ReadInput();
}
