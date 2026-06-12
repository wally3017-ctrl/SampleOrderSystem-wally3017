#pragma once
#include "IMenuController.h"
#include <iostream>
#include <string>

class MainController {
    IMenuController* sampleController_;
    IMenuController* orderController_;
    std::ostream&    output_;

public:
    MainController(IMenuController* sample,
                   IMenuController* order,
                   std::ostream&    output);

    // 입력 처리. false 반환 시 루프 종료
    bool ProcessInput(const std::string& input);

    void Run(std::istream& input = std::cin);
};
