#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>
#include <regex>
#include <stdexcept>
#include "Controller/OrderController.h"
#include "View/OrderView.h"
#include "Repository/MockRepository.h"

using ::testing::Return;
using ::testing::_;
using ::testing::Invoke;
using ::testing::SaveArg;

using MockOrderRepository  = MockRepository<Order>;
using MockSampleRepository = MockRepository<Sample>;

class MockProductionQueue : public IProductionQueue {
public:
    MOCK_METHOD(void, Enqueue, (const Order& order), (override));
};

struct OrderTestFixture {
    std::ostringstream out;
    std::istringstream in;
    ConsoleView        console{ out, in };
    OrderView          view{ console };
};

// ── Phase 3-1: 주문 접수 ──────────────────────────────────────────────────

TEST(OrderControllerTest, PlaceOrder_ValidSampleId_CreatesReservedOrder) {
    OrderTestFixture f;
    MockOrderRepository  mockOrderRepo;
    MockSampleRepository mockSampleRepo;
    MockProductionQueue  mockQueue;
    OrderController controller(mockOrderRepo, mockSampleRepo, mockQueue, f.view);

    Sample sample("S001", "GaN", 10, 0.9, 100);
    EXPECT_CALL(mockSampleRepo, Load("S001"))
        .WillOnce(Return(std::optional<Sample>(sample)));
    EXPECT_CALL(mockOrderRepo, LoadAll())
        .WillOnce(Return(std::vector<Order>{}));
    EXPECT_CALL(mockOrderRepo, Save(_)).Times(1);

    Order order = controller.PlaceOrder("S001", "고객A", 10);

    EXPECT_EQ(order.GetSampleId(),     "S001");
    EXPECT_EQ(order.GetCustomerName(), "고객A");
    EXPECT_EQ(order.GetQuantity(),     10);
    EXPECT_EQ(order.GetStatus(),       OrderStatus::RESERVED);
}

TEST(OrderControllerTest, PlaceOrder_InvalidSampleId_ThrowsException) {
    OrderTestFixture f;
    MockOrderRepository  mockOrderRepo;
    MockSampleRepository mockSampleRepo;
    MockProductionQueue  mockQueue;
    OrderController controller(mockOrderRepo, mockSampleRepo, mockQueue, f.view);

    EXPECT_CALL(mockSampleRepo, Load("INVALID"))
        .WillOnce(Return(std::optional<Sample>(std::nullopt)));

    EXPECT_THROW(controller.PlaceOrder("INVALID", "고객A", 10), std::invalid_argument);
}

TEST(OrderControllerTest, PlaceOrder_ZeroQuantity_ThrowsException) {
    OrderTestFixture f;
    MockOrderRepository  mockOrderRepo;
    MockSampleRepository mockSampleRepo;
    MockProductionQueue  mockQueue;
    OrderController controller(mockOrderRepo, mockSampleRepo, mockQueue, f.view);

    EXPECT_THROW(controller.PlaceOrder("S001", "고객A", 0), std::invalid_argument);
}

TEST(OrderControllerTest, GenerateOrderNumber_Format_MatchesOrdYYYYMMDDNNNN) {
    OrderTestFixture f;
    MockOrderRepository  mockOrderRepo;
    MockSampleRepository mockSampleRepo;
    MockProductionQueue  mockQueue;
    OrderController controller(mockOrderRepo, mockSampleRepo, mockQueue, f.view);

    Sample sample("S001", "GaN", 10, 0.9, 100);
    EXPECT_CALL(mockSampleRepo, Load("S001"))
        .WillOnce(Return(std::optional<Sample>(sample)));
    EXPECT_CALL(mockOrderRepo, LoadAll())
        .WillOnce(Return(std::vector<Order>{}));
    EXPECT_CALL(mockOrderRepo, Save(_)).Times(1);

    Order order = controller.PlaceOrder("S001", "고객A", 10);

    std::regex pattern(R"(ORD-\d{8}-\d{4})");
    EXPECT_TRUE(std::regex_match(order.GetId(), pattern));
}

TEST(OrderControllerTest, GenerateOrderNumber_MultipleOrders_NoDuplicate) {
    OrderTestFixture f;
    MockOrderRepository  mockOrderRepo;
    MockSampleRepository mockSampleRepo;
    MockProductionQueue  mockQueue;
    OrderController controller(mockOrderRepo, mockSampleRepo, mockQueue, f.view);

    Sample sample("S001", "GaN", 10, 0.9, 100);
    EXPECT_CALL(mockSampleRepo, Load("S001"))
        .WillRepeatedly(Return(std::optional<Sample>(sample)));

    std::vector<Order> savedOrders;
    EXPECT_CALL(mockOrderRepo, LoadAll())
        .WillRepeatedly(Invoke([&savedOrders]() { return savedOrders; }));
    EXPECT_CALL(mockOrderRepo, Save(_))
        .WillRepeatedly(Invoke([&savedOrders](const Order& o) { savedOrders.push_back(o); }));

    Order order1 = controller.PlaceOrder("S001", "고객A", 10);
    Order order2 = controller.PlaceOrder("S001", "고객B",  5);

    EXPECT_NE(order1.GetId(), order2.GetId());
}
