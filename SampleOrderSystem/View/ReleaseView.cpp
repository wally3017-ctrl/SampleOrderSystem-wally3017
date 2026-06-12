#include "ReleaseView.h"
#include "../Model/OrderStatus.h"

ReleaseView::ReleaseView(ConsoleView& console) : console_(console) {}

void ReleaseView::ShowMenu() {
    console_.PrintLine(" 1. 출고 처리    0. 메인 메뉴로 돌아가기");
    console_.Print(" 선택 >> ");
}

void ReleaseView::ShowReleasableList(const std::vector<Order>& orders) {
    console_.PrintSeparator();
    console_.PrintLine(" [출고 가능 목록]");
    console_.PrintLine(" 주문번호              시료ID    고객명           수량");
    console_.PrintLine(" " + std::string(54, '-'));
    for (const auto& order : orders) {
        console_.PrintLine(" " + order.GetId()
            + std::string(std::max(1, 22 - (int)order.GetId().size()), ' ')
            + order.GetSampleId()
            + std::string(std::max(1, 10 - (int)order.GetSampleId().size()), ' ')
            + order.GetCustomerName()
            + std::string(std::max(1, 17 - (int)order.GetCustomerName().size()), ' ')
            + std::to_string(order.GetQuantity()) + "ea");
    }
    console_.PrintSeparator();
}

void ReleaseView::ShowReleased(const Order& order) {
    console_.PrintLine(" [출고 완료] " + order.GetId()
        + " (고객: " + order.GetCustomerName()
        + "  수량: " + std::to_string(order.GetQuantity()) + "ea)");
    console_.PrintLine(" 해당 주문이 RELEASED 상태로 전환되었습니다.");
}

void ReleaseView::ShowEmptyList() {
    console_.PrintLine(" 출고 가능한 주문이 없습니다.");
}

void ReleaseView::ShowError(const std::string& msg) {
    console_.PrintLine(" [오류] " + msg);
}

std::string ReleaseView::ReadInput() {
    return console_.ReadLine();
}

std::string ReleaseView::ReadOrderId() {
    console_.Print(" 출고할 주문번호 >> ");
    return console_.ReadLine();
}
