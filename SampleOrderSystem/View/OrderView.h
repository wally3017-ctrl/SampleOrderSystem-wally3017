#pragma once
#include "ConsoleView.h"
#include "../Model/Order.h"
#include <string>
#include <vector>

class OrderView {
    ConsoleView& console_;
public:
    explicit OrderView(ConsoleView& console);

    void ShowMenu();
    void ShowReservedList(const std::vector<Order>& orders);
    void ShowApprovalActions();
    void ShowPlacedOrder(const Order& order);
    void ShowApprovalResult(const Order& order);
    void ShowRejectionResult(const Order& order);
    void ShowError(const std::string& msg);

    std::string ReadInput();
    std::string ReadSampleId();
    std::string ReadCustomerName();
    int         ReadQuantity();
    std::string ReadOrderId();
};
