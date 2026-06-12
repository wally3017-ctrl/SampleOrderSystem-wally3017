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

void OrderController::Run() {
    // Phase 3-4에서 구현
}
