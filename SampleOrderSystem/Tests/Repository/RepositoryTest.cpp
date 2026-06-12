#include <filesystem>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Repository/IRepository.h"
#include "Repository/MockRepository.h"
#include "Repository/JsonRepository.h"

struct TestEntity {
    std::string id;
    std::string name;

    std::string GetId() const { return id; }

    JsonObject ToJson() const {
        JsonObject obj;
        obj.Set("id", id);
        obj.Set("name", name);
        return obj;
    }

    static TestEntity FromJson(const JsonObject& obj) {
        return TestEntity{ obj.GetString("id"), obj.GetString("name") };
    }

    bool operator==(const TestEntity& other) const {
        return id == other.id && name == other.name;
    }
};

class RepositoryTest : public ::testing::Test {
protected:
    const std::string testFilePath = "test_repository.json";

    void SetUp() override {
        std::filesystem::remove(testFilePath);
    }

    void TearDown() override {
        std::filesystem::remove(testFilePath);
    }
};

TEST_F(RepositoryTest, Save_ValidEntity_StoresSuccessfully) {
    JsonRepository<TestEntity> repo(testFilePath);
    TestEntity entity{ "id1", "TestSample" };

    repo.Save(entity);
    auto result = repo.Load("id1");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->id, entity.id);
    EXPECT_EQ(result->name, entity.name);
}

TEST_F(RepositoryTest, Load_ExistingId_ReturnsEntity) {
    {
        JsonRepository<TestEntity> repo(testFilePath);
        repo.Save(TestEntity{ "id2", "LoadTest" });
    }
    // 재시작 후 파일에서 불러오는지 검증
    JsonRepository<TestEntity> repo2(testFilePath);
    auto result = repo2.Load("id2");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->id, "id2");
    EXPECT_EQ(result->name, "LoadTest");
}

TEST_F(RepositoryTest, Load_NonExistingId_ReturnsNull) {
    JsonRepository<TestEntity> repo(testFilePath);

    auto result = repo.Load("nonexistent");

    EXPECT_FALSE(result.has_value());
}

TEST_F(RepositoryTest, Update_ExistingEntity_UpdatesSuccessfully) {
    JsonRepository<TestEntity> repo(testFilePath);
    repo.Save(TestEntity{ "id3", "Original" });

    repo.Update(TestEntity{ "id3", "Updated" });
    auto result = repo.Load("id3");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->name, "Updated");
}

TEST_F(RepositoryTest, Delete_ExistingId_RemovesEntity) {
    JsonRepository<TestEntity> repo(testFilePath);
    repo.Save(TestEntity{ "id4", "ToDelete" });

    repo.Delete("id4");
    auto result = repo.Load("id4");

    EXPECT_FALSE(result.has_value());
}
