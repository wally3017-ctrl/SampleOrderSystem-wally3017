#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdexcept>
#include "Controller/SampleController.h"
#include "Repository/MockRepository.h"

using ::testing::Return;
using ::testing::_;

using MockSampleRepository = MockRepository<Sample>;

// Phase 2-1 [Red]: SampleController::Register 미구현 - 모두 실패 상태

TEST(SampleControllerTest, RegisterSample_ValidInput_SavesSuccessfully) {
    MockSampleRepository mockRepo;
    SampleController controller(mockRepo);
    Sample sample("S001", "TestSample", 10, 0.9);

    EXPECT_CALL(mockRepo, Load("S001"))
        .WillOnce(Return(std::optional<Sample>(std::nullopt)));
    EXPECT_CALL(mockRepo, Save(sample)).Times(1);

    EXPECT_NO_THROW(controller.Register(sample));
}

TEST(SampleControllerTest, RegisterSample_DuplicateId_ThrowsException) {
    MockSampleRepository mockRepo;
    SampleController controller(mockRepo);
    Sample existing ("S001", "Existing",  10, 0.9);
    Sample duplicate("S001", "Duplicate",  5, 0.8);

    EXPECT_CALL(mockRepo, Load("S001"))
        .WillOnce(Return(std::optional<Sample>(existing)));

    EXPECT_THROW(controller.Register(duplicate), std::invalid_argument);
}

TEST(SampleControllerTest, RegisterSample_YieldZero_ThrowsException) {
    MockSampleRepository mockRepo;
    SampleController controller(mockRepo);
    Sample sample("S002", "ZeroYield", 10, 0.0);

    EXPECT_THROW(controller.Register(sample), std::invalid_argument);
}

TEST(SampleControllerTest, RegisterSample_YieldOverOne_ThrowsException) {
    MockSampleRepository mockRepo;
    SampleController controller(mockRepo);
    Sample sample("S003", "OverYield", 10, 1.1);

    EXPECT_THROW(controller.Register(sample), std::invalid_argument);
}

TEST(SampleControllerTest, RegisterSample_NegativeProductionTime_ThrowsException) {
    MockSampleRepository mockRepo;
    SampleController controller(mockRepo);
    Sample sample("S004", "NegTime", -1, 0.9);

    EXPECT_THROW(controller.Register(sample), std::invalid_argument);
}
