#pragma once
#include <optional>
#include <string>
#include <vector>

template<typename T>
class IRepository {
public:
    virtual ~IRepository() = default;
    virtual void Save(const T& entity) = 0;
    virtual std::optional<T> Load(const std::string& id) const = 0;
    virtual std::vector<T> LoadAll() const = 0;
    virtual void Update(const T& entity) = 0;
    virtual void Delete(const std::string& id) = 0;
};
