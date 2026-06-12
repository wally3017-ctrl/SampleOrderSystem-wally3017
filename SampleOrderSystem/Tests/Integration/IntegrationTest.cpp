#include <gtest/gtest.h>
#include <sstream>
#include "InMemoryRepository.h"
#include "Controller/OrderController.h"
#include "Controller/ProductionController.h"
#include "Controller/ReleaseController.h"
#include "Controller/MonitoringService.h"
#include "Production/ProductionQueue.h"
#include "View/ConsoleView.h"
#include "View/OrderView.h"
#include "View/ProductionView.h"
#include "View/ReleaseView.h"

struct IntegrationFixture {
    InMemoryRepository<Sample> sampleRepo;
    InMemoryRepository<Order>  orderRepo;
    ProductionQueue            queue{ "" };

    std::ostringstream out;
    std::istringstream in;
    ConsoleView        console{ out, in };

    OrderView      orderView{ console };
    ProductionView productionView{ console };
    ReleaseView    releaseView{ console };

    OrderController      orderCtrl{ orderRepo, sampleRepo, queue, orderView };
    ProductionController productionCtrl{ queue, orderRepo, sampleRepo, productionView };
    ReleaseController    releaseCtrl{ orderRepo, releaseView };
    MonitoringService    monitoringSvc{ orderRepo, sampleRepo };
};

// ── Phase 5-5: 통합 시나리오 테스트 ─────────────────────────────────────────

TEST(IntegrationTest, Scenario_OrderToRelease_SufficientStock_FullFlow) {
    IntegrationFixture f;

    f.sampleRepo.Save(Sample("S001", "GaN", 10, 0.9, 100));

    Order order = f.orderCtrl.PlaceOrder("S001", "고객A", 50);
    EXPECT_EQ(order.GetStatus(), OrderStatus::RESERVED);

    f.orderCtrl.Approve(order.GetId());
    auto approved = f.orderRepo.Load(order.GetId());
    ASSERT_TRUE(approved.has_value());
    EXPECT_EQ(approved->GetStatus(), OrderStatus::CONFIRMED);

    // 재고 50 차감 확인
    auto sample = f.sampleRepo.Load("S001");
    ASSERT_TRUE(sample.has_value());
    EXPECT_EQ(sample->GetStock(), 50);

    f.releaseCtrl.Release(order.GetId());
    auto released = f.orderRepo.Load(order.GetId());
    ASSERT_TRUE(released.has_value());
    EXPECT_EQ(released->GetStatus(), OrderStatus::RELEASED);
}

TEST(IntegrationTest, Scenario_OrderToRelease_InsufficientStock_FullFlow) {
    IntegrationFixture f;

    f.sampleRepo.Save(Sample("S001", "GaN", 10, 0.9, 3));

    Order order = f.orderCtrl.PlaceOrder("S001", "고객A", 10);
    f.orderCtrl.Approve(order.GetId());

    auto producing = f.orderRepo.Load(order.GetId());
    ASSERT_TRUE(producing.has_value());
    EXPECT_EQ(producing->GetStatus(), OrderStatus::PRODUCING);
    EXPECT_EQ(f.queue.GetQueue().size(), 1u);

    f.productionCtrl.Complete();

    auto confirmed = f.orderRepo.Load(order.GetId());
    ASSERT_TRUE(confirmed.has_value());
    EXPECT_EQ(confirmed->GetStatus(), OrderStatus::CONFIRMED);
    EXPECT_TRUE(f.queue.GetQueue().empty());

    f.releaseCtrl.Release(order.GetId());
    auto released = f.orderRepo.Load(order.GetId());
    ASSERT_TRUE(released.has_value());
    EXPECT_EQ(released->GetStatus(), OrderStatus::RELEASED);
}

TEST(IntegrationTest, Scenario_RejectOrder_ExcludedFromMonitoring) {
    IntegrationFixture f;

    f.sampleRepo.Save(Sample("S001", "GaN", 10, 0.9, 100));

    Order order = f.orderCtrl.PlaceOrder("S001", "고객A", 50);
    f.orderCtrl.Reject(order.GetId());

    auto rejected = f.orderRepo.Load(order.GetId());
    ASSERT_TRUE(rejected.has_value());
    EXPECT_EQ(rejected->GetStatus(), OrderStatus::REJECTED);

    auto summary = f.monitoringSvc.GetOrderSummary();
    EXPECT_EQ(summary.reserved,  0);
    EXPECT_EQ(summary.confirmed, 0);
    EXPECT_EQ(summary.producing, 0);
    EXPECT_EQ(summary.released,  0);
}

TEST(IntegrationTest, Scenario_MultipleOrders_FIFOProductionOrder) {
    IntegrationFixture f;

    f.sampleRepo.Save(Sample("S001", "GaN", 10, 0.9, 0));

    Order o1 = f.orderCtrl.PlaceOrder("S001", "고객A", 5);
    Order o2 = f.orderCtrl.PlaceOrder("S001", "고객B", 5);
    Order o3 = f.orderCtrl.PlaceOrder("S001", "고객C", 5);

    f.orderCtrl.Approve(o1.GetId());
    f.orderCtrl.Approve(o2.GetId());
    f.orderCtrl.Approve(o3.GetId());

    auto queue = f.queue.GetQueue();
    ASSERT_EQ(queue.size(), 3u);
    EXPECT_EQ(queue[0].GetOrderId(), o1.GetId());
    EXPECT_EQ(queue[1].GetOrderId(), o2.GetId());
    EXPECT_EQ(queue[2].GetOrderId(), o3.GetId());

    f.productionCtrl.Complete();

    auto o1Status = f.orderRepo.Load(o1.GetId());
    ASSERT_TRUE(o1Status.has_value());
    EXPECT_EQ(o1Status->GetStatus(), OrderStatus::CONFIRMED);

    auto remaining = f.queue.GetQueue();
    ASSERT_EQ(remaining.size(), 2u);
    EXPECT_EQ(remaining[0].GetOrderId(), o2.GetId());
}
