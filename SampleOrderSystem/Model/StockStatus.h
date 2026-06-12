#pragma once
#include <string>

enum class StockStatus { YEOYOO, BUJOK, GODAL };

inline std::string ToString(StockStatus s) {
    switch (s) {
    case StockStatus::YEOYOO: return "여유";
    case StockStatus::BUJOK:  return "부족";
    case StockStatus::GODAL:  return "고갈";
    default:                  return "알 수 없음";
    }
}
