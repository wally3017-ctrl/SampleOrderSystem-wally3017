#pragma once
#include "ConsoleView.h"
#include "../Model/Order.h"
#include <vector>
#include <string>

class ReleaseView {
    ConsoleView& console_;
public:
    explicit ReleaseView(ConsoleView& console);
    void ShowMenu();
    void ShowReleasableList(const std::vector<Order>& orders);
    void ShowReleased(const Order& order);
    void ShowEmptyList();
    void ShowError(const std::string& msg);
    std::string ReadInput();
    std::string ReadOrderId();
};
