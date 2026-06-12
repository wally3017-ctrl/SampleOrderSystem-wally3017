#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Controller/MonitoringService.h"
#include "Repository/MockRepository.h"

using MockOrderRepository  = MockRepository<Order>;
using MockSampleRepository = MockRepository<Sample>;
using ::testing::Return;

// ── Phase 5-1: 모니터링 집계 테스트 ─────────────────────────────────────────

TEST(MonitoringServiceTest, GetOrderSummary_ExcludesRejected_ReturnsCorrectCounts) {
    MockOrderRepository  mockOrderRepo;
    MockSampleRepository mockSampleRepo;
    MonitoringService service(mockOrderRepo, mockSampleRepo);

    std::vector<Order> orders = {
        Order("O1", "S1", "A", 5, OrderStatus::RESERVED,  "2026-06-12 09:00:00"),
        Order("O2", "S1", "B", 3, OrderStatus::REJECTED,  "2026-06-12 09:00:00"),
        Order("O3", "S1", "C", 2, OrderStatus::CONFIRMED, "2026-06-12 09:00:00"),
    };
    EXPECT_CALL(mockOrderRepo, LoadAll()).WillOnce(Return(orders));

    auto summary = service.GetOrderSummary();
    EXPECT_EQ(summary.reserved,  1);
    EXPECT_EQ(summary.confirmed, 1);
    EXPECT_EQ(summary.producing, 0);
    EXPECT_EQ(summary.released,  0);
}

TEST(MonitoringServiceTest, GetOrderSummary_AllStatuses_CountsIndependently) {
    MockOrderRepository  mockOrderRepo;
    MockSampleRepository mockSampleRepo;
    MonitoringService service(mockOrderRepo, mockSampleRepo);

    std::vector<Order> orders = {
        Order("O1", "S1", "A", 5, OrderStatus::RESERVED,  "2026-06-12 09:00:00"),
        Order("O2", "S1", "B", 3, OrderStatus::CONFIRMED, "2026-06-12 09:00:00"),
        Order("O3", "S1", "C", 2, OrderStatus::PRODUCING, "2026-06-12 09:00:00"),
        Order("O4", "S1", "D", 1, OrderStatus::RELEASED,  "2026-06-12 09:00:00"),
        Order("O5", "S1", "E", 4, OrderStatus::REJECTED,  "2026-06-12 09:00:00"),
    };
    EXPECT_CALL(mockOrderRepo, LoadAll()).WillOnce(Return(orders));

    auto summary = service.GetOrderSummary();
    EXPECT_EQ(summary.reserved,  1);
    EXPECT_EQ(summary.confirmed, 1);
    EXPECT_EQ(summary.producing, 1);
    EXPECT_EQ(summary.released,  1);
}

TEST(MonitoringServiceTest, GetStockStatus_StockIsZero_ReturnsGodal) {
    EXPECT_EQ(MonitoringService::CalcStockStatus(0, 5), StockStatus::GODAL);
}

TEST(MonitoringServiceTest, GetStockStatus_StockLessThanOrdered_ReturnsBujok) {
    EXPECT_EQ(MonitoringService::CalcStockStatus(3, 10), StockStatus::BUJOK);
}

TEST(MonitoringServiceTest, GetStockStatus_StockSufficient_ReturnsYeoyoo) {
    EXPECT_EQ(MonitoringService::CalcStockStatus(10, 5), StockStatus::YEOYOO);
}
