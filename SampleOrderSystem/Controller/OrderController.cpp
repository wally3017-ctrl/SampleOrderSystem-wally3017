#include "OrderController.h"
#include "OrderNumberGenerator.h"
#include "MenuKey.h"
#include "../Production/ProductionCalculator.h"
#include <ctime>
#include <stdexcept>

namespace {
    constexpr std::string_view SUB_PLACE   = "1";
    constexpr std::string_view SUB_APPROVE = "2";
    constexpr std::string_view ACT_APPROVE = "1";
    constexpr std::string_view ACT_REJECT  = "2";

    Order LoadOrderOrThrow(IRepository<Order>& repo, const std::string& orderId) {
        auto opt = repo.Load(orderId);
        if (!opt.has_value())
            throw std::invalid_argument("존재하지 않는 주문 번호입니다: " + orderId);
        return opt.value();
    }

    Sample LoadSampleOrThrow(IRepository<Sample>& repo, const std::string& sampleId) {
        auto opt = repo.Load(sampleId);
        if (!opt.has_value())
            throw std::invalid_argument("존재하지 않는 시료 ID입니다: " + sampleId);
        return opt.value();
    }

    void AssertReservedStatus(const Order& order, std::string_view action) {
        if (order.GetStatus() != OrderStatus::RESERVED)
            throw std::invalid_argument(
                std::string("RESERVED 상태의 주문만 ") + std::string(action) + "할 수 있습니다.");
    }

    bool HasSufficientStock(const Sample& sample, int quantity) {
        return sample.GetStock() >= quantity;
    }

    std::string CurrentTimestamp() {
        time_t t = time(nullptr);
        tm tm_info{};
        localtime_s(&tm_info, &t);
        char buf[20];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm_info);
        return buf;
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
    LoadSampleOrThrow(sampleRepo_, sampleId);

    OrderNumberGenerator generator(orderRepo_);
    Order order(generator.Generate(), sampleId, customerName,
                quantity, OrderStatus::RESERVED, CurrentTimestamp());
    orderRepo_.Save(order);
    return order;
}

void OrderController::Approve(const std::string& orderId) {
    Order order = LoadOrderOrThrow(orderRepo_, orderId);
    AssertReservedStatus(order, "승인");

    Sample sample = LoadSampleOrThrow(sampleRepo_, order.GetSampleId());
    if (HasSufficientStock(sample, order.GetQuantity())) {
        sample.SetStock(sample.GetStock() - order.GetQuantity());
        sampleRepo_.Update(sample);
        order.SetStatus(OrderStatus::CONFIRMED);
    } else {
        int shortage  = order.GetQuantity() - sample.GetStock();
        int actualQty = ProductionCalculator::CalcActualProduction(shortage, sample.GetYield());
        int totalTime = ProductionCalculator::CalcTotalTime(sample.GetAvgProductionTime(), actualQty);
        ProductionJob job(order.GetId(), order.GetSampleId(),
                          shortage, actualQty, totalTime, CurrentTimestamp());
        order.SetStatus(OrderStatus::PRODUCING);
        productionQueue_.Enqueue(job);
    }
    orderRepo_.Update(order);
}

void OrderController::Reject(const std::string& orderId) {
    Order order = LoadOrderOrThrow(orderRepo_, orderId);
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
