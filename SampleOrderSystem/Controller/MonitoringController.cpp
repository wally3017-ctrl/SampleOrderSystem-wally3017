#include "MonitoringController.h"
#include "../View/ConsoleView.h"

MonitoringController::MonitoringController(MonitoringService& service, MonitoringView& view)
    : service_(service), view_(view)
{}

void MonitoringController::Run() {
    auto summary    = service_.GetOrderSummary();
    auto stockInfos = service_.GetStockInfoAll();
    view_.ShowMonitoring(summary, stockInfos);
}
