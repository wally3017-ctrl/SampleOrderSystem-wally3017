#include "MainController.h"

// Phase 1-3 [Red]: 스텁 - 라우팅 미구현 상태로 모든 테스트 실패

MainController::MainController(IMenuController* sample,
                               IMenuController* order,
                               std::ostream&    output)
    : sampleController_(sample)
    , orderController_(order)
    , output_(output)
{}

bool MainController::ProcessInput(const std::string& input) {
    return true; // 항상 계속 → 0 입력 시 종료 테스트 실패
}

void MainController::Run(std::istream& input) {
    std::string line;
    while (std::getline(input, line)) {
        if (!ProcessInput(line)) break;
    }
}
