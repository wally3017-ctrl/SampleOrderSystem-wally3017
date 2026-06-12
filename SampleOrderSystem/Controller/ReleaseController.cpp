#include "ReleaseController.h"
#include <stdexcept>

ReleaseController::ReleaseController(IRepository<Order>& orderRepo, ReleaseView& view)
    : orderRepo_(orderRepo), view_(view)
{}

void ReleaseController::Release(const std::string& orderId) {
    auto opt = orderRepo_.Load(orderId);
    if (!opt.has_value())
        throw std::invalid_argument("존재하지 않는 주문 번호입니다: " + orderId);

    Order order = opt.value();
    if (order.GetStatus() != OrderStatus::CONFIRMED)
        throw std::invalid_argument("CONFIRMED 상태의 주문만 출고할 수 있습니다.");

    order.SetStatus(OrderStatus::RELEASED);
    orderRepo_.Update(order);
}

std::vector<Order> ReleaseController::GetReleasableOrders() const {
    std::vector<Order> result;
    for (const auto& order : orderRepo_.LoadAll())
        if (order.GetStatus() == OrderStatus::CONFIRMED)
            result.push_back(order);
    return result;
}

void ReleaseController::Run() {
    while (true) {
        auto releasable = GetReleasableOrders();
        view_.ShowReleasableList(releasable);

        if (releasable.empty()) {
            view_.ShowEmptyList();
            break;
        }

        view_.ShowMenu();
        std::string input = view_.ReadInput();
        if (input == "0") break;

        try {
            std::string orderId = view_.ReadOrderId();
            Release(orderId);
            auto opt = orderRepo_.Load(orderId);
            if (opt.has_value()) view_.ShowReleased(opt.value());
        } catch (const std::invalid_argument& e) {
            view_.ShowError(e.what());
        }
    }
}
