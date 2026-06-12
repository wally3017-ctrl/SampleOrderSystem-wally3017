#pragma once
#include "IMenuController.h"
#include "../View/ConsoleView.h"
#include <string>

class MainController {
    IMenuController* sampleController_;
    IMenuController* orderController_;
    ConsoleView&     console_;

public:
    MainController(IMenuController* sample,
                   IMenuController* order,
                   ConsoleView&     console);

    bool ProcessInput(const std::string& input);

    void Run(std::istream& input = std::cin);
};
