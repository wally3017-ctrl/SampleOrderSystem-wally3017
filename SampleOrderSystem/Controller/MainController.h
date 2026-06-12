#pragma once
#include "IMenuController.h"
#include "../View/ConsoleView.h"
#include <map>
#include <string>

class MainController {
    std::map<std::string, IMenuController*> menuMap_;
    ConsoleView& console_;

public:
    explicit MainController(ConsoleView& console);

    void Register(const std::string& key, IMenuController* controller);
    bool ProcessInput(const std::string& input);
};
