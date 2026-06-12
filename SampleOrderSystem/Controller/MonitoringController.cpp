#include "MonitoringController.h"
#include "../View/ConsoleView.h"

MonitoringController::MonitoringController(MonitoringService& service, MonitoringView& view)
    : service_(service), view_(view)
{}

void MonitoringController::Run() {
    view_.ShowMonitoring(service_.GetOrderSummary(), service_.GetStockInfoAll());
    view_.ReadInput();
}
