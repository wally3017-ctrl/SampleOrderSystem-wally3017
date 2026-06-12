#pragma once
#include "IRepository.h"
#include <stdexcept>
#include <string>

template<typename T>
T LoadOrThrow(IRepository<T>& repo, const std::string& id, const std::string& errorMsg) {
    auto opt = repo.Load(id);
    if (!opt.has_value())
        throw std::invalid_argument(errorMsg + id);
    return opt.value();
}
