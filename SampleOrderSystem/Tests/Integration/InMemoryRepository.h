#pragma once
#include "Repository/IRepository.h"
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

template<typename T>
class InMemoryRepository : public IRepository<T> {
    std::map<std::string, T> data_;
public:
    void Save(const T& entity) override {
        data_[entity.GetId()] = entity;
    }
    std::optional<T> Load(const std::string& id) const override {
        auto it = data_.find(id);
        return it == data_.end() ? std::nullopt : std::optional<T>(it->second);
    }
    std::vector<T> LoadAll() const override {
        std::vector<T> result;
        result.reserve(data_.size());
        for (const auto& [id, e] : data_) result.push_back(e);
        return result;
    }
    void Update(const T& entity) override {
        if (!data_.contains(entity.GetId()))
            throw std::runtime_error("엔티티를 찾을 수 없습니다: " + entity.GetId());
        data_[entity.GetId()] = entity;
    }
    void Delete(const std::string& id) override {
        data_.erase(id);
    }
};
