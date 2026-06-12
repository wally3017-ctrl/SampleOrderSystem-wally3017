#include "MainController.h"
#include "MenuKey.h"

MainController::MainController(ConsoleView& console)
    : console_(console)
{}

void MainController::Register(const std::string& key, IMenuController* controller) {
    menuMap_[key] = controller;
}

bool MainController::ProcessInput(const std::string& input) {
    if (input == MenuKey::EXIT) return false;

    auto it = menuMap_.find(input);
    if (it != menuMap_.end()) {
        it->second->Run();
        return true;
    }

    console_.PrintError("잘못된 메뉴 번호입니다: " + input);
    return true;
}
