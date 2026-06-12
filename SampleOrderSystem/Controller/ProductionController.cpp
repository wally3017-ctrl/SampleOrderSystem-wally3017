#include "ProductionController.h"
#include <ctime>
#include <cstdio>
#include <stdexcept>

namespace {
    bool IsProductionComplete(const ProductionJob& job) {
        tm tm_info{};
        const std::string& ts = job.GetEnqueuedAt();
        sscanf_s(ts.c_str(), "%d-%d-%d %d:%d:%d",
                 &tm_info.tm_year, &tm_info.tm_mon,  &tm_info.tm_mday,
                 &tm_info.tm_hour, &tm_info.tm_min,  &tm_info.tm_sec);
        tm_info.tm_year -= 1900;
        tm_info.tm_mon  -= 1;
        tm_info.tm_isdst = -1;
        time_t enqueued   = mktime(&tm_info);
        time_t completion = enqueued + static_cast<time_t>(job.GetTotalTime()) * 60;
        return time(nullptr) >= completion;
    }
}

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
        if (!opt.has_value() || !IsProductionComplete(opt.value())) break;
        ProductionJob job = opt.value();
        Complete();
        view_.ShowAutoCompleted(job);
    }
}

void ProductionController::Run() {
    AutoComplete();
    while (true) {
        auto all     = queue_.GetQueue();
        auto current = all.empty()
            ? std::optional<ProductionJob>{}
            : std::optional<ProductionJob>(all.front());
        std::vector<ProductionJob> waiting(
            all.size() > 1 ? all.begin() + 1 : all.end(), all.end());

        view_.ShowQueue(current, waiting);

        if (!current.has_value()) {
            view_.ShowEmptyQueue();
            break;
        }

        view_.ShowProductionMenu();
        std::string input = view_.ReadInput();
        if (input == "0") break;
        if (input == "1") {
            ProductionJob completedJob = current.value();
            Complete();
            view_.ShowCompleted(completedJob);
        }
    }
}
