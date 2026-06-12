#pragma once
#include "IRepository.h"
#include <string>

// Phase 1-1 [Red]: stub - all tests expected to fail
template<typename T>
class JsonRepository : public IRepository<T> {
public:
    explicit JsonRepository(const std::string& filePath) {}

    void Save(const T& entity) override {}

    std::optional<T> Load(const std::string& id) const override {
        return std::nullopt;
    }

    std::vector<T> LoadAll() const override {
        return {};
    }

    void Update(const T& entity) override {}

    void Delete(const std::string& id) override {}
};
