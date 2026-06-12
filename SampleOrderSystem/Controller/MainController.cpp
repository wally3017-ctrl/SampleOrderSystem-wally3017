#include "MainController.h"

MainController::MainController(IMenuController* sample,
                               IMenuController* order,
                               std::ostream&    output)
    : sampleController_(sample)
    , orderController_(order)
    , output_(output)
{}

bool MainController::ProcessInput(const std::string& input) {
    if (input == "0") return false;
    if (input == "1") { sampleController_->Run(); return true; }
    if (input == "2") { orderController_->Run(); return true; }

    output_ << "[오류] 잘못된 메뉴 번호입니다: " << input << "\n";
    return true;
}

void MainController::Run(std::istream& input) {
    std::string line;
    while (std::getline(input, line)) {
        if (!ProcessInput(line)) break;
    }
}
