#include <gtest/gtest.h>
#include "Production/ProductionQueue.h"

namespace {
    ProductionJob MakeJob(const std::string& orderId, const std::string& sampleId = "S001") {
        return ProductionJob(orderId, sampleId, 5, 7, 70, "2026-06-12 09:00:00");
    }
}

// ── Phase 4-3: FIFO 생산 큐 ───────────────────────────────────────────────

TEST(ProductionQueueTest, Enqueue_MultipleJobs_MaintainsFIFOOrder) {
    ProductionQueue queue;
    queue.Enqueue(MakeJob("ORD-001"));
    queue.Enqueue(MakeJob("ORD-002"));
    queue.Enqueue(MakeJob("ORD-003"));

    auto first = queue.Dequeue();
    ASSERT_TRUE(first.has_value());
    EXPECT_EQ(first->GetOrderId(), "ORD-001");

    auto second = queue.Dequeue();
    ASSERT_TRUE(second.has_value());
    EXPECT_EQ(second->GetOrderId(), "ORD-002");
}

TEST(ProductionQueueTest, Dequeue_EmptyQueue_ReturnsNullopt) {
    ProductionQueue queue;

    auto result = queue.Dequeue();

    EXPECT_FALSE(result.has_value());
}

TEST(ProductionQueueTest, Peek_NonEmptyQueue_ReturnsFirstWithoutRemoving) {
    ProductionQueue queue;
    queue.Enqueue(MakeJob("ORD-001"));
    queue.Enqueue(MakeJob("ORD-002"));

    auto peeked = queue.Peek();
    ASSERT_TRUE(peeked.has_value());
    EXPECT_EQ(peeked->GetOrderId(), "ORD-001");

    EXPECT_EQ(queue.Size(), 2u);
}

TEST(ProductionQueueTest, GetQueue_ReturnsAllJobsInOrder) {
    ProductionQueue queue;
    queue.Enqueue(MakeJob("ORD-001"));
    queue.Enqueue(MakeJob("ORD-002"));
    queue.Enqueue(MakeJob("ORD-003"));

    auto all = queue.GetQueue();

    ASSERT_EQ(all.size(), 3u);
    EXPECT_EQ(all[0].GetOrderId(), "ORD-001");
    EXPECT_EQ(all[1].GetOrderId(), "ORD-002");
    EXPECT_EQ(all[2].GetOrderId(), "ORD-003");
}
