#pragma once
#include "ConsoleView.h"
#include "../Model/SystemSummary.h"

class MainMenuView {
    ConsoleView& console_;

public:
    explicit MainMenuView(ConsoleView& console);
    void Show(const SystemSummary& summary = {});
};
