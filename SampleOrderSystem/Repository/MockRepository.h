#pragma once
#include <gmock/gmock.h>
#include "IRepository.h"

template<typename T>
class MockRepository : public IRepository<T> {
public:
    MOCK_METHOD(void, Save, (const T& entity), (override));
    MOCK_METHOD(std::optional<T>, Load, (const std::string& id), (const, override));
    MOCK_METHOD(std::vector<T>, LoadAll, (), (const, override));
    MOCK_METHOD(void, Update, (const T& entity), (override));
    MOCK_METHOD(void, Delete, (const std::string& id), (override));
};
