#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Repository/IRepository.h"
#include "Repository/MockRepository.h"

using ::testing::Return;
using ::testing::Eq;

struct Item {
    std::string id;
    std::string name;
    std::string GetId() const { return id; }
    bool operator==(const Item& o) const { return id == o.id && name == o.name; }
};

// 1. EXPECT_CALL - 호출 횟수 검증
TEST(MockRepositoryTest, Save_CalledOnce_VerifiesCallCount) {
    MockRepository<Item> mock;
    Item item{ "A1", "SampleA" };

    EXPECT_CALL(mock, Save(Eq(item))).Times(1);

    mock.Save(item);
    // 테스트 종료 시 gmock이 Times(1) 충족 여부를 자동 검증
}

// 2. WillOnce(Return) - 반환값 지정
TEST(MockRepositoryTest, Load_ReturnsConfiguredValue) {
    MockRepository<Item> mock;
    Item item{ "B1", "SampleB" };

    EXPECT_CALL(mock, Load("B1"))
        .WillOnce(Return(std::optional<Item>(item)));

    auto result = mock.Load("B1");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->id, "B1");
    EXPECT_EQ(result->name, "SampleB");
}

// 3. 존재하지 않는 ID - nullopt 반환 지정
TEST(MockRepositoryTest, Load_NonExistingId_ReturnsNullopt) {
    MockRepository<Item> mock;

    EXPECT_CALL(mock, Load("NONE"))
        .WillOnce(Return(std::optional<Item>(std::nullopt)));

    auto result = mock.Load("NONE");

    EXPECT_FALSE(result.has_value());
}

// 4. LoadAll - 다건 반환 지정
TEST(MockRepositoryTest, LoadAll_ReturnsAllConfiguredItems) {
    MockRepository<Item> mock;
    std::vector<Item> items{ {"C1","S1"}, {"C2","S2"}, {"C3","S3"} };

    EXPECT_CALL(mock, LoadAll())
        .WillOnce(Return(items));

    auto result = mock.LoadAll();

    EXPECT_EQ(result.size(), 3u);
    EXPECT_EQ(result[0].id, "C1");
    EXPECT_EQ(result[2].id, "C3");
}

// 5. Delete - 올바른 인자로 호출됐는지 검증
TEST(MockRepositoryTest, Delete_CalledWithCorrectId) {
    MockRepository<Item> mock;

    EXPECT_CALL(mock, Delete("D1")).Times(1);

    mock.Delete("D1");
}
