#pragma once
#include "IMenuController.h"
#include "MonitoringService.h"
#include "../View/MonitoringView.h"

class MonitoringController : public IMenuController {
    MonitoringService& service_;
    MonitoringView&    view_;
public:
    MonitoringController(MonitoringService& service, MonitoringView& view);
    void Run() override;
};
