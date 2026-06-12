#include "MainController.h"
#include "MenuKey.h"

MainController::MainController(IMenuController* sample,
                               IMenuController* order,
                               ConsoleView&     console)
    : sampleController_(sample)
    , orderController_(order)
    , console_(console)
{}

bool MainController::ProcessInput(const std::string& input) {
    if (input == MenuKey::EXIT)   return false;
    if (input == MenuKey::SAMPLE) { sampleController_->Run(); return true; }
    if (input == MenuKey::ORDER)  { orderController_->Run();  return true; }

    console_.PrintError("잘못된 메뉴 번호입니다: " + input);
    return true;
}

void MainController::Run(std::istream& input) {
    std::string line;
    while (std::getline(input, line)) {
        if (!ProcessInput(line)) break;
    }
}
