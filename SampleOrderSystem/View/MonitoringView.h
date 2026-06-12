#pragma once
#include "ConsoleView.h"
#include "../Controller/MonitoringService.h"
#include <vector>

class MonitoringView {
    ConsoleView& console_;
public:
    explicit MonitoringView(ConsoleView& console);
    void ShowMonitoring(const OrderSummary& summary,
                        const std::vector<SampleStockInfo>& stockInfos);
};
