#pragma once
#include <string>
#include <stdexcept>

enum class OrderStatus { RESERVED, REJECTED, PRODUCING, CONFIRMED, RELEASED };

inline std::string ToString(OrderStatus s) {
    switch (s) {
    case OrderStatus::RESERVED:  return "RESERVED";
    case OrderStatus::REJECTED:  return "REJECTED";
    case OrderStatus::PRODUCING: return "PRODUCING";
    case OrderStatus::CONFIRMED: return "CONFIRMED";
    case OrderStatus::RELEASED:  return "RELEASED";
    default:                     return "UNKNOWN";
    }
}

inline OrderStatus OrderStatusFromString(const std::string& s) {
    if (s == "RESERVED")  return OrderStatus::RESERVED;
    if (s == "REJECTED")  return OrderStatus::REJECTED;
    if (s == "PRODUCING") return OrderStatus::PRODUCING;
    if (s == "CONFIRMED") return OrderStatus::CONFIRMED;
    if (s == "RELEASED")  return OrderStatus::RELEASED;
    throw std::invalid_argument("알 수 없는 주문 상태: " + s);
}
