#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>
#include "Controller/ReleaseController.h"
#include "Repository/MockRepository.h"

using MockOrderRepository  = MockRepository<Order>;
using MockSampleRepository = MockRepository<Sample>;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::_;

struct ReleaseTestFixture {
    std::ostringstream out;
    std::istringstream in;
    ConsoleView        console{ out, in };
    ReleaseView        view{ console };
};

// ── Phase 5-3: 출고 처리 테스트 ──────────────────────────────────────────────

TEST(ReleaseControllerTest, Release_ConfirmedOrder_ChangesToReleased) {
    ReleaseTestFixture  f;
    MockOrderRepository  mockOrderRepo;
    MockSampleRepository mockSampleRepo;
    ReleaseController   controller(mockOrderRepo, mockSampleRepo, f.view);

    Order  confirmed("ORD-001", "S001", "고객A", 10, OrderStatus::CONFIRMED, "2026-06-12 09:00:00");
    Sample sample("S001", "GaN", 10, 0.9, 100);

    EXPECT_CALL(mockOrderRepo,  Load("ORD-001")).WillOnce(Return(std::optional<Order>(confirmed)));
    EXPECT_CALL(mockSampleRepo, Load("S001")).WillOnce(Return(std::optional<Sample>(sample)));

    Order  capturedOrder;
    Sample capturedSample;
    EXPECT_CALL(mockOrderRepo,  Update(_)).WillOnce(SaveArg<0>(&capturedOrder));
    EXPECT_CALL(mockSampleRepo, Update(_)).WillOnce(SaveArg<0>(&capturedSample));

    controller.Release("ORD-001");

    EXPECT_EQ(capturedOrder.GetStatus(),  OrderStatus::RELEASED);
    EXPECT_EQ(capturedSample.GetStock(),  90);
}

TEST(ReleaseControllerTest, Release_NonConfirmedOrder_ThrowsException) {
    ReleaseTestFixture  f;
    MockOrderRepository  mockOrderRepo;
    MockSampleRepository mockSampleRepo;
    ReleaseController   controller(mockOrderRepo, mockSampleRepo, f.view);

    Order reserved("ORD-001", "S001", "고객A", 10, OrderStatus::RESERVED, "2026-06-12 09:00:00");
    EXPECT_CALL(mockOrderRepo, Load("ORD-001")).WillOnce(Return(std::optional<Order>(reserved)));

    EXPECT_THROW(controller.Release("ORD-001"), std::invalid_argument);
}

TEST(ReleaseControllerTest, GetReleasableOrders_ReturnsOnlyConfirmed) {
    ReleaseTestFixture  f;
    MockOrderRepository  mockOrderRepo;
    MockSampleRepository mockSampleRepo;
    ReleaseController   controller(mockOrderRepo, mockSampleRepo, f.view);

    std::vector<Order> orders = {
        Order("O1", "S1", "A", 5, OrderStatus::CONFIRMED, "2026-06-12 09:00:00"),
        Order("O2", "S1", "B", 3, OrderStatus::RESERVED,  "2026-06-12 09:00:00"),
        Order("O3", "S1", "C", 2, OrderStatus::PRODUCING, "2026-06-12 09:00:00"),
        Order("O4", "S1", "D", 1, OrderStatus::RELEASED,  "2026-06-12 09:00:00"),
    };
    EXPECT_CALL(mockOrderRepo, LoadAll()).WillOnce(Return(orders));

    auto result = controller.GetReleasableOrders();
    ASSERT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0].GetId(), "O1");
}

TEST(ReleaseControllerTest, GetReleasableOrders_NoConfirmedOrders_ReturnsEmptyList) {
    ReleaseTestFixture  f;
    MockOrderRepository  mockOrderRepo;
    MockSampleRepository mockSampleRepo;
    ReleaseController   controller(mockOrderRepo, mockSampleRepo, f.view);

    std::vector<Order> orders = {
        Order("O1", "S1", "A", 5, OrderStatus::RESERVED,  "2026-06-12 09:00:00"),
        Order("O2", "S1", "B", 3, OrderStatus::PRODUCING, "2026-06-12 09:00:00"),
    };
    EXPECT_CALL(mockOrderRepo, LoadAll()).WillOnce(Return(orders));

    auto result = controller.GetReleasableOrders();
    EXPECT_TRUE(result.empty());
}
