#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Repository/IRepository.h"
#include "Repository/MockRepository.h"
#include "Repository/JsonRepository.h"

struct TestEntity {
    std::string id;
    std::string name;
    std::string GetId() const { return id; }
    bool operator==(const TestEntity& other) const {
        return id == other.id && name == other.name;
    }
};

// Phase 1-1 [Red]: tests expected to fail - JsonRepository is stub only

TEST(RepositoryTest, Save_ValidEntity_StoresSuccessfully) {
    JsonRepository<TestEntity> repo("test_repo.json");
    TestEntity entity{ "id1", "TestSample" };

    repo.Save(entity);
    auto result = repo.Load("id1");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->id, entity.id);
    EXPECT_EQ(result->name, entity.name);
}

TEST(RepositoryTest, Load_ExistingId_ReturnsEntity) {
    JsonRepository<TestEntity> repo("test_repo.json");
    TestEntity entity{ "id2", "LoadTest" };
    repo.Save(entity);

    auto result = repo.Load("id2");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->id, "id2");
    EXPECT_EQ(result->name, "LoadTest");
}

TEST(RepositoryTest, Load_NonExistingId_ReturnsNull) {
    JsonRepository<TestEntity> repo("test_repo.json");

    auto result = repo.Load("nonexistent");

    EXPECT_FALSE(result.has_value());
}

TEST(RepositoryTest, Update_ExistingEntity_UpdatesSuccessfully) {
    JsonRepository<TestEntity> repo("test_repo.json");
    TestEntity entity{ "id3", "Original" };
    repo.Save(entity);

    entity.name = "Updated";
    repo.Update(entity);
    auto result = repo.Load("id3");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->name, "Updated");
}

TEST(RepositoryTest, Delete_ExistingId_RemovesEntity) {
    JsonRepository<TestEntity> repo("test_repo.json");
    TestEntity entity{ "id4", "ToDelete" };
    repo.Save(entity);

    repo.Delete("id4");
    auto result = repo.Load("id4");

    EXPECT_FALSE(result.has_value());
}
