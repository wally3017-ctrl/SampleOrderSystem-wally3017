#pragma once
#include "ConsoleView.h"

struct SystemSummary {
    int sampleCount         = 0;
    int totalStock          = 0;
    int totalOrders         = 0;
    int productionQueueCount = 0;
};

class MainMenuView {
    ConsoleView& console_;

public:
    explicit MainMenuView(ConsoleView& console);
    void Show(const SystemSummary& summary = {});
};
