#pragma once
#include "IRepository.h"
#include "Persistable.h"
#include "../Utility/JsonObject.h"
#include <filesystem>
#include <fstream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

template<Persistable T>
class JsonRepository : public IRepository<T> {
    std::string filePath_;
    std::map<std::string, T> cache_;

    void LoadFromFile() {
        cache_.clear();
        if (!std::filesystem::exists(filePath_)) return;
        std::ifstream file(filePath_);
        if (!file.is_open()) return;
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            try {
                auto obj    = JsonObject::Deserialize(line);
                auto entity = T::FromJson(obj);
                cache_[entity.GetId()] = entity;
            } catch (...) {}
        }
    }

    void FlushToFile() const {
        std::ofstream file(filePath_);
        if (!file.is_open())
            throw std::runtime_error("파일을 열 수 없습니다: " + filePath_);
        for (const auto& [id, entity] : cache_)
            file << entity.ToJson().Serialize() << "\n";
    }

public:
    explicit JsonRepository(const std::string& filePath) : filePath_(filePath) {
        LoadFromFile();
    }

    void Save(const T& entity) override {
        cache_[entity.GetId()] = entity;
        FlushToFile();
    }

    std::optional<T> Load(const std::string& id) const override {
        auto it = cache_.find(id);
        if (it == cache_.end()) return std::nullopt;
        return it->second;
    }

    std::vector<T> LoadAll() const override {
        std::vector<T> result;
        result.reserve(cache_.size());
        for (const auto& [id, entity] : cache_)
            result.push_back(entity);
        return result;
    }

    void Update(const T& entity) override {
        if (!cache_.contains(entity.GetId()))
            throw std::runtime_error("엔티티를 찾을 수 없습니다: " + entity.GetId());
        Save(entity);
    }

    void Delete(const std::string& id) override {
        cache_.erase(id);
        FlushToFile();
    }
};
