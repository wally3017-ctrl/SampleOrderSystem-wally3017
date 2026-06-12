#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>
#include "Controller/ProductionController.h"
#include "Repository/MockRepository.h"

using ::testing::Return;
using ::testing::_;
using ::testing::SaveArg;

using MockOrderRepository  = MockRepository<Order>;
using MockSampleRepository = MockRepository<Sample>;

class MockProductionQueue : public IProductionQueue {
public:
    MOCK_METHOD(void, Enqueue, (const ProductionJob& job), (override));
    MOCK_METHOD(std::optional<ProductionJob>, Dequeue, (), (override));
    MOCK_METHOD(std::optional<ProductionJob>, Peek, (), (const, override));
    MOCK_METHOD(std::vector<ProductionJob>,   GetQueue, (), (const, override));
};

struct ProductionTestFixture {
    std::ostringstream out;
    std::istringstream in;
    ConsoleView        console{ out, in };
    ProductionView     view{ console };
};

namespace {
    ProductionJob MakeJob(const std::string& orderId = "ORD-001",
                          const std::string& sampleId = "S001",
                          int actualQty = 7) {
        return ProductionJob(orderId, sampleId, 5, actualQty, 70, "2026-06-12 09:00:00");
    }
}

// ── Phase 4-5: 생산 완료 처리 ─────────────────────────────────────────────

TEST(ProductionControllerTest, CompleteProduction_ChangesOrderToConfirmed) {
    ProductionTestFixture f;
    MockProductionQueue  mockQueue;
    MockOrderRepository  mockOrderRepo;
    MockSampleRepository mockSampleRepo;
    ProductionController controller(mockQueue, mockOrderRepo, mockSampleRepo, f.view);

    ProductionJob job  = MakeJob();
    Order  order("ORD-001", "S001", "고객A", 10, OrderStatus::PRODUCING,  "2026-06-12 09:00:00");
    Sample sample("S001", "GaN", 10, 0.9, 0);

    EXPECT_CALL(mockQueue,      Dequeue()).WillOnce(Return(std::optional<ProductionJob>(job)));
    EXPECT_CALL(mockOrderRepo,  Load("ORD-001")).WillOnce(Return(std::optional<Order>(order)));
    EXPECT_CALL(mockSampleRepo, Load("S001")).WillOnce(Return(std::optional<Sample>(sample)));
    EXPECT_CALL(mockSampleRepo, Update(_));

    Order capturedOrder;
    EXPECT_CALL(mockOrderRepo, Update(_)).WillOnce(SaveArg<0>(&capturedOrder));

    controller.Complete();

    EXPECT_EQ(capturedOrder.GetStatus(), OrderStatus::CONFIRMED);
}

TEST(ProductionControllerTest, CompleteProduction_IncreasesStockByActualProduction) {
    ProductionTestFixture f;
    MockProductionQueue  mockQueue;
    MockOrderRepository  mockOrderRepo;
    MockSampleRepository mockSampleRepo;
    ProductionController controller(mockQueue, mockOrderRepo, mockSampleRepo, f.view);

    ProductionJob job  = MakeJob("ORD-001", "S001", 7);
    Order  order("ORD-001", "S001", "고객A", 10, OrderStatus::PRODUCING,  "2026-06-12 09:00:00");
    Sample sample("S001", "GaN", 10, 0.9, 0);

    EXPECT_CALL(mockQueue,      Dequeue()).WillOnce(Return(std::optional<ProductionJob>(job)));
    EXPECT_CALL(mockOrderRepo,  Load("ORD-001")).WillOnce(Return(std::optional<Order>(order)));
    EXPECT_CALL(mockSampleRepo, Load("S001")).WillOnce(Return(std::optional<Sample>(sample)));
    EXPECT_CALL(mockOrderRepo,  Update(_));

    Sample capturedSample;
    EXPECT_CALL(mockSampleRepo, Update(_)).WillOnce(SaveArg<0>(&capturedSample));

    controller.Complete();

    EXPECT_EQ(capturedSample.GetStock(), 7);
}

TEST(ProductionControllerTest, CompleteProduction_RemovesJobFromQueue) {
    ProductionTestFixture f;
    MockProductionQueue  mockQueue;
    MockOrderRepository  mockOrderRepo;
    MockSampleRepository mockSampleRepo;
    ProductionController controller(mockQueue, mockOrderRepo, mockSampleRepo, f.view);

    ProductionJob job  = MakeJob();
    Order  order("ORD-001", "S001", "고객A", 10, OrderStatus::PRODUCING,  "2026-06-12 09:00:00");
    Sample sample("S001", "GaN", 10, 0.9, 0);

    EXPECT_CALL(mockQueue, Dequeue()).Times(1).WillOnce(Return(std::optional<ProductionJob>(job)));
    EXPECT_CALL(mockOrderRepo,  Load(_)).WillRepeatedly(Return(std::optional<Order>(order)));
    EXPECT_CALL(mockSampleRepo, Load(_)).WillRepeatedly(Return(std::optional<Sample>(sample)));
    EXPECT_CALL(mockOrderRepo,  Update(_));
    EXPECT_CALL(mockSampleRepo, Update(_));

    controller.Complete();
}

TEST(ProductionControllerTest, CompleteProduction_EmptyQueue_NoAction) {
    ProductionTestFixture f;
    MockProductionQueue  mockQueue;
    MockOrderRepository  mockOrderRepo;
    MockSampleRepository mockSampleRepo;
    ProductionController controller(mockQueue, mockOrderRepo, mockSampleRepo, f.view);

    EXPECT_CALL(mockQueue,      Dequeue()).WillOnce(Return(std::optional<ProductionJob>(std::nullopt)));
    EXPECT_CALL(mockOrderRepo,  Load(_)).Times(0);
    EXPECT_CALL(mockSampleRepo, Load(_)).Times(0);

    EXPECT_NO_THROW(controller.Complete());
}
