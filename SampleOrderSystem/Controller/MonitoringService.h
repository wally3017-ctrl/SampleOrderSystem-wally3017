#pragma once
#include "../Model/Order.h"
#include "../Model/Sample.h"
#include "../Model/StockStatus.h"
#include "../Repository/IRepository.h"
#include <string>
#include <vector>

struct OrderSummary {
    int reserved  = 0;
    int confirmed = 0;
    int producing = 0;
    int released  = 0;
};

struct SampleStockInfo {
    std::string sampleId;
    std::string sampleName;
    int         stock;
    int         orderedQty;
    StockStatus status;
};

class MonitoringService {
    IRepository<Order>&  orderRepo_;
    IRepository<Sample>& sampleRepo_;
public:
    MonitoringService(IRepository<Order>& orderRepo, IRepository<Sample>& sampleRepo);
    OrderSummary                 GetOrderSummary()  const;
    std::vector<SampleStockInfo> GetStockInfoAll()  const;
    static StockStatus           CalcStockStatus(int stock, int orderedQty);
};
