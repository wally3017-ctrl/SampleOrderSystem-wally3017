#include "MonitoringService.h"
#include <unordered_map>

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

    std::unordered_map<std::string, int> reservedQtyMap;
    for (const auto& order : orders)
        if (order.GetStatus() == OrderStatus::RESERVED)
            reservedQtyMap[order.GetSampleId()] += order.GetQuantity();

    std::vector<SampleStockInfo> result;
    result.reserve(samples.size());
    for (const auto& sample : samples) {
        int orderedQty = 0;
        if (auto it = reservedQtyMap.find(sample.GetId()); it != reservedQtyMap.end())
            orderedQty = it->second;
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

SystemSummary MonitoringService::GetSystemSummary(int productionQueueCount) const {
    SystemSummary summary;
    for (const auto& s : sampleRepo_.LoadAll()) {
        summary.sampleCount++;
        summary.totalStock += s.GetStock();
    }
    summary.totalOrders          = static_cast<int>(orderRepo_.LoadAll().size());
    summary.productionQueueCount = productionQueueCount;
    return summary;
}

StockStatus MonitoringService::CalcStockStatus(int stock, int orderedQty) {
    if (stock == 0)         return StockStatus::GODAL;
    if (stock < orderedQty) return StockStatus::BUJOK;
    return StockStatus::YEOYOO;
}
