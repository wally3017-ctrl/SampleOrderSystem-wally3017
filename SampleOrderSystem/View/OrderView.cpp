#include "OrderView.h"
#include <stdexcept>

OrderView::OrderView(ConsoleView& console) : console_(console) {}

void OrderView::ShowMenu() {
    // Phase 3-4에서 구현
}

void OrderView::ShowReservedList(const std::vector<Order>&) {
    // Phase 3-4에서 구현
}

void OrderView::ShowPlacedOrder(const Order&) {
    // Phase 3-4에서 구현
}

void OrderView::ShowApprovalResult(const Order&) {
    // Phase 3-4에서 구현
}

void OrderView::ShowRejectionResult(const Order&) {
    // Phase 3-4에서 구현
}

void OrderView::ShowError(const std::string& msg) {
    console_.PrintError(msg);
}

std::string OrderView::ReadInput() {
    return console_.ReadLine();
}

std::string OrderView::ReadSampleId() {
    console_.Print(" 시료 ID    : ");
    return console_.ReadLine();
}

std::string OrderView::ReadCustomerName() {
    console_.Print(" 고객명     : ");
    return console_.ReadLine();
}

int OrderView::ReadQuantity() {
    console_.Print(" 주문 수량  : ");
    std::string line = console_.ReadLine();
    try { return std::stoi(line); }
    catch (...) { throw std::invalid_argument("숫자를 입력해야 합니다."); }
}

std::string OrderView::ReadOrderId() {
    console_.Print(" 주문 번호  : ");
    return console_.ReadLine();
}
