#include "OrderView.h"
#include "ViewUtils.h"
#include <stdexcept>

namespace {
    constexpr int COL_ORDERID  = 20;
    constexpr int COL_SAMPLEID =  8;
    constexpr int COL_CUSTOMER = 14;
    constexpr int TABLE_WIDTH  = 54;

    std::string FormatOrderRow(const Order& o) {
        return " "
            + ViewUtils::PadTo(o.GetId(),           COL_ORDERID)
            + ViewUtils::PadTo(o.GetSampleId(),      COL_SAMPLEID)
            + ViewUtils::PadTo(o.GetCustomerName(),  COL_CUSTOMER)
            + std::to_string(o.GetQuantity()) + "개";
    }
}

OrderView::OrderView(ConsoleView& console) : console_(console) {}

void OrderView::ShowMenu() {
    console_.PrintSeparator();
    console_.PrintLine("                  주문 관리");
    console_.PrintSeparator();
    console_.PrintLine(" 1. 시료 주문 접수");
    console_.PrintLine(" 2. 주문 승인/거절");
    console_.PrintLine(" 0. 메인 메뉴로 돌아가기");
    console_.PrintSeparator();
    console_.Print(" 메뉴 선택 >> ");
}

void OrderView::ShowReservedList(const std::vector<Order>& orders) {
    console_.PrintSeparator();
    console_.PrintLine(" [접수 대기 주문 목록]");
    console_.PrintSeparator();
    if (orders.empty()) {
        console_.PrintLine(" 처리 대기 중인 주문이 없습니다.");
    } else {
        console_.PrintLine(" 주문번호            시료ID  고객명        수량");
        console_.PrintLine(" " + std::string(TABLE_WIDTH, '-'));
        for (const auto& o : orders)
            console_.PrintLine(FormatOrderRow(o));
    }
    console_.PrintSeparator();
}

void OrderView::ShowApprovalActions() {
    console_.Print(" 처리 선택 (1. 승인  2. 거절) >> ");
}

void OrderView::ShowPlacedOrder(const Order& order) {
    console_.PrintLine(" 주문이 접수되었습니다.");
    console_.PrintLine(" 주문번호 : " + order.GetId());
    console_.PrintLine(" 시료 ID  : " + order.GetSampleId());
    console_.PrintLine(" 고객명   : " + order.GetCustomerName());
    console_.PrintLine(" 수량     : " + std::to_string(order.GetQuantity()) + "개");
    console_.PrintLine(" 상태     : " + ToString(order.GetStatus()));
}

void OrderView::ShowApprovalResult(const Order& order) {
    console_.PrintLine(" [승인 완료] " + order.GetId()
        + " → " + ToString(order.GetStatus()));
}

void OrderView::ShowRejectionResult(const Order& order) {
    console_.PrintLine(" [거절 완료] " + order.GetId()
        + " → " + ToString(order.GetStatus()));
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
