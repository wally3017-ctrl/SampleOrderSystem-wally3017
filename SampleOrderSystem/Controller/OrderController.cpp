#include "OrderController.h"
#include "OrderNumberGenerator.h"
#include "MenuKey.h"
#include "../Production/ProductionCalculator.h"
#include "../Repository/RepositoryUtils.h"
#include "../Utility/Timestamp.h"
#include <stdexcept>

namespace {
    constexpr std::string_view SUB_PLACE   = "1";
    constexpr std::string_view SUB_APPROVE = "2";
    constexpr std::string_view ACT_APPROVE = "1";
    constexpr std::string_view ACT_REJECT  = "2";

    void AssertReservedStatus(const Order& order, std::string_view action) {
        if (order.GetStatus() != OrderStatus::RESERVED)
            throw std::invalid_argument(
                std::string("RESERVED 상태의 주문만 ") + std::string(action) + "할 수 있습니다.");
    }

    int CalcConfirmedQty(IRepository<Order>& repo, const std::string& sampleId) {
        int total = 0;
        for (const auto& o : repo.LoadAll())
            if (o.GetSampleId() == sampleId && o.GetStatus() == OrderStatus::CONFIRMED)
                total += o.GetQuantity();
        return total;
    }
}

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
    LoadOrThrow(sampleRepo_, sampleId, "존재하지 않는 시료 ID입니다: ");

    OrderNumberGenerator generator(orderRepo_);
    Order order(generator.Generate(), sampleId, customerName,
                quantity, OrderStatus::RESERVED, Timestamp::Now());
    orderRepo_.Save(order);
    return order;
}

void OrderController::Approve(const std::string& orderId) {
    Order order = LoadOrThrow(orderRepo_, orderId, "존재하지 않는 주문 번호입니다: ");
    AssertReservedStatus(order, "승인");

    Sample sample         = LoadOrThrow(sampleRepo_, order.GetSampleId(), "존재하지 않는 시료 ID입니다: ");
    int    confirmedQty   = CalcConfirmedQty(orderRepo_, order.GetSampleId());
    int    availableStock = sample.GetStock() - confirmedQty;

    if (availableStock >= order.GetQuantity()) {
        order.SetStatus(OrderStatus::CONFIRMED);
    } else {
        int shortage  = order.GetQuantity() - std::max(0, availableStock);
        int actualQty = ProductionCalculator::CalcActualProduction(shortage, sample.GetYield());
        int totalTime = ProductionCalculator::CalcTotalTime(sample.GetAvgProductionTime(), actualQty);
        ProductionJob job(order.GetId(), order.GetSampleId(),
                          shortage, actualQty, totalTime, Timestamp::Now());
        order.SetStatus(OrderStatus::PRODUCING);
        productionQueue_.Enqueue(job);
    }
    orderRepo_.Update(order);
}

void OrderController::Reject(const std::string& orderId) {
    Order order = LoadOrThrow(orderRepo_, orderId, "존재하지 않는 주문 번호입니다: ");
    AssertReservedStatus(order, "거절");
    order.SetStatus(OrderStatus::REJECTED);
    orderRepo_.Update(order);
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
                view_.ShowPlacedOrder(PlaceOrder(sampleId, customerName, quantity));
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
