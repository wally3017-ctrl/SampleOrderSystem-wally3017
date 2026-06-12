#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>
#include <stdexcept>
#include "Controller/SampleController.h"
#include "View/SampleView.h"
#include "Repository/MockRepository.h"

using ::testing::Return;
using ::testing::_;

using MockSampleRepository = MockRepository<Sample>;

// 테스트용 ConsoleView + SampleView 헬퍼
struct SampleTestFixture {
    std::ostringstream out;
    std::istringstream in;
    ConsoleView        console{ out, in };
    SampleView         view{ console };
};

TEST(SampleControllerTest, RegisterSample_ValidInput_SavesSuccessfully) {
    SampleTestFixture f;
    MockSampleRepository mockRepo;
    SampleController controller(mockRepo, f.view);
    Sample sample("S001", "TestSample", 10, 0.9);

    EXPECT_CALL(mockRepo, Load("S001"))
        .WillOnce(Return(std::optional<Sample>(std::nullopt)));
    EXPECT_CALL(mockRepo, Save(sample)).Times(1);

    EXPECT_NO_THROW(controller.Register(sample));
}

TEST(SampleControllerTest, RegisterSample_DuplicateId_ThrowsException) {
    SampleTestFixture f;
    MockSampleRepository mockRepo;
    SampleController controller(mockRepo, f.view);
    Sample existing ("S001", "Existing",  10, 0.9);
    Sample duplicate("S001", "Duplicate",  5, 0.8);

    EXPECT_CALL(mockRepo, Load("S001"))
        .WillOnce(Return(std::optional<Sample>(existing)));

    EXPECT_THROW(controller.Register(duplicate), std::invalid_argument);
}

TEST(SampleControllerTest, RegisterSample_YieldZero_ThrowsException) {
    SampleTestFixture f;
    MockSampleRepository mockRepo;
    SampleController controller(mockRepo, f.view);
    Sample sample("S002", "ZeroYield", 10, 0.0);

    EXPECT_THROW(controller.Register(sample), std::invalid_argument);
}

TEST(SampleControllerTest, RegisterSample_YieldOverOne_ThrowsException) {
    SampleTestFixture f;
    MockSampleRepository mockRepo;
    SampleController controller(mockRepo, f.view);
    Sample sample("S003", "OverYield", 10, 1.1);

    EXPECT_THROW(controller.Register(sample), std::invalid_argument);
}

TEST(SampleControllerTest, RegisterSample_NegativeProductionTime_ThrowsException) {
    SampleTestFixture f;
    MockSampleRepository mockRepo;
    SampleController controller(mockRepo, f.view);
    Sample sample("S004", "NegTime", -1, 0.9);

    EXPECT_THROW(controller.Register(sample), std::invalid_argument);
}

TEST(SampleControllerTest, GetAllSamples_EmptyRepository_ReturnsEmptyList) {
    SampleTestFixture f;
    MockSampleRepository mockRepo;
    SampleController controller(mockRepo, f.view);

    EXPECT_CALL(mockRepo, LoadAll())
        .WillOnce(Return(std::vector<Sample>{}));

    auto result = controller.GetAll();
    EXPECT_TRUE(result.empty());
}

TEST(SampleControllerTest, GetAllSamples_MultipleSamples_ReturnsAllWithStock) {
    SampleTestFixture f;
    MockSampleRepository mockRepo;
    SampleController controller(mockRepo, f.view);
    std::vector<Sample> samples{
        {"S001", "SampleA", 10, 0.9, 100},
        {"S002", "SampleB",  5, 0.8,  50},
    };

    EXPECT_CALL(mockRepo, LoadAll())
        .WillOnce(Return(samples));

    auto result = controller.GetAll();
    EXPECT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0].GetStock(), 100);
    EXPECT_EQ(result[1].GetStock(),  50);
}

TEST(SampleControllerTest, SearchSample_ExactName_ReturnsMatch) {
    SampleTestFixture f;
    MockSampleRepository mockRepo;
    SampleController controller(mockRepo, f.view);
    std::vector<Sample> samples{
        {"S001", "GaN",  10, 0.9, 100},
        {"S002", "SiC",   5, 0.8,  50},
    };

    EXPECT_CALL(mockRepo, LoadAll())
        .WillOnce(Return(samples));

    auto result = controller.Search("GaN");
    ASSERT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0].GetName(), "GaN");
}

TEST(SampleControllerTest, SearchSample_PartialName_ReturnsMatches) {
    SampleTestFixture f;
    MockSampleRepository mockRepo;
    SampleController controller(mockRepo, f.view);
    std::vector<Sample> samples{
        {"S001", "GaN-001", 10, 0.9, 100},
        {"S002", "GaN-002",  5, 0.8,  50},
        {"S003", "SiC-001",  8, 0.7,  30},
    };

    EXPECT_CALL(mockRepo, LoadAll())
        .WillOnce(Return(samples));

    auto result = controller.Search("GaN");
    EXPECT_EQ(result.size(), 2u);
}

TEST(SampleControllerTest, SearchSample_NoMatch_ReturnsEmptyList) {
    SampleTestFixture f;
    MockSampleRepository mockRepo;
    SampleController controller(mockRepo, f.view);
    std::vector<Sample> samples{
        {"S001", "GaN", 10, 0.9, 100},
    };

    EXPECT_CALL(mockRepo, LoadAll())
        .WillOnce(Return(samples));

    auto result = controller.Search("SiC");
    EXPECT_TRUE(result.empty());
}
