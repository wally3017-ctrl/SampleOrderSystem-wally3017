#pragma once
#include "../Model/Order.h"
#include "../Model/Sample.h"
#include "../Model/OrderSummary.h"
#include "../Model/SampleStockInfo.h"
#include "../Model/SystemSummary.h"
#include "../Repository/IRepository.h"
#include <vector>

class MonitoringService {
    IRepository<Order>&  orderRepo_;
    IRepository<Sample>& sampleRepo_;
public:
    MonitoringService(IRepository<Order>& orderRepo, IRepository<Sample>& sampleRepo);
    OrderSummary                 GetOrderSummary()              const;
    std::vector<SampleStockInfo> GetStockInfoAll()              const;
    SystemSummary                GetSystemSummary(int productionQueueCount) const;
    static StockStatus           CalcStockStatus(int stock, int orderedQty);
};
