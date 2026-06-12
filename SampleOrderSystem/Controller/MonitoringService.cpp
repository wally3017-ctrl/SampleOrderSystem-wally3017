#include "MonitoringService.h"

MonitoringService::MonitoringService(IRepository<Order>& orderRepo, IRepository<Sample>& sampleRepo)
    : orderRepo_(orderRepo), sampleRepo_(sampleRepo)
{}

OrderSummary MonitoringService::GetOrderSummary() const {
    OrderSummary summary;
    for (const auto& order : orderRepo_.LoadAll()) {
        switch (order.GetStatus()) {
        case OrderStatus::RESERVED:  summary.reserved++;  break;
        case OrderStatus::CONFIRMED: summary.confirmed++; break;
        case OrderStatus::PRODUCING: summary.producing++; break;
        case OrderStatus::RELEASED:  summary.released++;  break;
        case OrderStatus::REJECTED:                       break;
        }
    }
    return summary;
}

std::vector<SampleStockInfo> MonitoringService::GetStockInfoAll() const {
    auto orders  = orderRepo_.LoadAll();
    auto samples = sampleRepo_.LoadAll();

    std::vector<SampleStockInfo> result;
    result.reserve(samples.size());

    for (const auto& sample : samples) {
        int orderedQty = 0;
        for (const auto& order : orders) {
            if (order.GetSampleId() == sample.GetId()
                && order.GetStatus() == OrderStatus::RESERVED)
                orderedQty += order.GetQuantity();
        }
        result.push_back({
            sample.GetId(),
            sample.GetName(),
            sample.GetStock(),
            orderedQty,
            CalcStockStatus(sample.GetStock(), orderedQty)
        });
    }
    return result;
}

StockStatus MonitoringService::CalcStockStatus(int stock, int orderedQty) {
    if (stock == 0)         return StockStatus::GODAL;
    if (stock < orderedQty) return StockStatus::BUJOK;
    return StockStatus::YEOYOO;
}
