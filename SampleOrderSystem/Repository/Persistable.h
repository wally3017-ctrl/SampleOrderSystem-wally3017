#pragma once
#include "../Utility/JsonObject.h"
#include <concepts>
#include <string>

template<typename T>
concept Persistable = requires(const T& t, const JsonObject& j) {
    { t.GetId() }      -> std::convertible_to<std::string>;
    { t.ToJson() }     -> std::convertible_to<JsonObject>;
    { T::FromJson(j) } -> std::same_as<T>;
};
