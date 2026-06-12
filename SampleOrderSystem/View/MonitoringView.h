#pragma once
#include "ConsoleView.h"
#include "../Model/OrderSummary.h"
#include "../Model/SampleStockInfo.h"
#include <vector>

class MonitoringView {
    ConsoleView& console_;
public:
    explicit MonitoringView(ConsoleView& console);
    void ShowMonitoring(const OrderSummary& summary,
                        const std::vector<SampleStockInfo>& stockInfos);
    std::string ReadInput();
};
