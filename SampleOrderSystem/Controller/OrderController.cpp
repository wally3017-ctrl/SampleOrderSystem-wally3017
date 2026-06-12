#include "OrderController.h"
#include "OrderNumberGenerator.h"
#include "MenuKey.h"
#include <ctime>
#include <stdexcept>

OrderController::OrderController(IRepository<Order>&  orderRepo,
                                 IRepository<Sample>& sampleRepo,
                                 IProductionQueue&    productionQueue,
                                 OrderView&           view)
    : orderRepo_(orderRepo)
    , sampleRepo_(sampleRepo)
    , productionQueue_(productionQueue)
    , view_(view)
{}

Order OrderController::PlaceOrder(const std::string& sampleId,
                                   const std::string& customerName,
                                   int quantity) {
    if (quantity <= 0)
        throw std::invalid_argument("주문 수량은 1 이상이어야 합니다.");
    if (!sampleRepo_.Load(sampleId).has_value())
        throw std::invalid_argument("존재하지 않는 시료 ID입니다: " + sampleId);

    OrderNumberGenerator generator(orderRepo_);
    std::string orderId = generator.Generate();

    time_t t = time(nullptr);
    tm tm_info{};
    localtime_s(&tm_info, &t);
    char timeBuf[20];
    strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", &tm_info);

    Order order(orderId, sampleId, customerName, quantity, OrderStatus::RESERVED, timeBuf);
    orderRepo_.Save(order);
    return order;
}

void OrderController::Approve(const std::string& orderId) {
    auto orderOpt = orderRepo_.Load(orderId);
    if (!orderOpt.has_value())
        throw std::invalid_argument("존재하지 않는 주문 번호입니다: " + orderId);

    Order order = orderOpt.value();
    if (order.GetStatus() != OrderStatus::RESERVED)
        throw std::invalid_argument("RESERVED 상태의 주문만 승인할 수 있습니다.");

    auto sampleOpt = sampleRepo_.Load(order.GetSampleId());
    if (!sampleOpt.has_value())
        throw std::invalid_argument("시료를 찾을 수 없습니다: " + order.GetSampleId());

    Sample sample = sampleOpt.value();
    if (sample.GetStock() >= order.GetQuantity()) {
        sample.SetStock(sample.GetStock() - order.GetQuantity());
        sampleRepo_.Update(sample);
        order.SetStatus(OrderStatus::CONFIRMED);
    } else {
        order.SetStatus(OrderStatus::PRODUCING);
        productionQueue_.Enqueue(order);
    }
    orderRepo_.Update(order);
}

void OrderController::Reject(const std::string& orderId) {
    auto orderOpt = orderRepo_.Load(orderId);
    if (!orderOpt.has_value())
        throw std::invalid_argument("존재하지 않는 주문 번호입니다: " + orderId);

    Order order = orderOpt.value();
    if (order.GetStatus() != OrderStatus::RESERVED)
        throw std::invalid_argument("RESERVED 상태의 주문만 거절할 수 있습니다.");

    order.SetStatus(OrderStatus::REJECTED);
    orderRepo_.Update(order);
}

namespace {
    constexpr std::string_view SUB_PLACE   = "1";
    constexpr std::string_view SUB_APPROVE = "2";
    constexpr std::string_view ACT_APPROVE = "1";
    constexpr std::string_view ACT_REJECT  = "2";
}

void OrderController::Run() {
    std::string input;
    while (true) {
        view_.ShowMenu();
        input = view_.ReadInput();
        if (input == MenuKey::EXIT) break;

        if (input == SUB_PLACE) {
            try {
                std::string sampleId     = view_.ReadSampleId();
                std::string customerName = view_.ReadCustomerName();
                int         quantity     = view_.ReadQuantity();
                Order order = PlaceOrder(sampleId, customerName, quantity);
                view_.ShowPlacedOrder(order);
            } catch (const std::invalid_argument& e) {
                view_.ShowError(e.what());
            }
        } else if (input == SUB_APPROVE) {
            auto all = orderRepo_.LoadAll();
            std::vector<Order> reserved;
            for (const auto& o : all)
                if (o.GetStatus() == OrderStatus::RESERVED)
                    reserved.push_back(o);
            view_.ShowReservedList(reserved);
            if (reserved.empty()) continue;

            try {
                std::string orderId = view_.ReadOrderId();
                view_.ShowApprovalActions();
                std::string action = view_.ReadInput();
                if (action == ACT_APPROVE) {
                    Approve(orderId);
                    auto opt = orderRepo_.Load(orderId);
                    if (opt.has_value()) view_.ShowApprovalResult(opt.value());
                } else if (action == ACT_REJECT) {
                    Reject(orderId);
                    auto opt = orderRepo_.Load(orderId);
                    if (opt.has_value()) view_.ShowRejectionResult(opt.value());
                }
            } catch (const std::invalid_argument& e) {
                view_.ShowError(e.what());
            }
        }
    }
}
