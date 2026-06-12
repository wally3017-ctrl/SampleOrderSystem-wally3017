#include "OrderNumberGenerator.h"
#include <ctime>
#include <cstdio>
#include <algorithm>

namespace {
    std::string GetTodayPrefix() {
        time_t t = time(nullptr);
        tm tm_info{};
        localtime_s(&tm_info, &t);
        char buf[9];
        strftime(buf, sizeof(buf), "%Y%m%d", &tm_info);
        return std::string("ORD-") + buf + "-";
    }

    int ExtractSequence(const std::string& orderId, const std::string& prefix) {
        if (orderId.size() <= prefix.size() || orderId.substr(0, prefix.size()) != prefix)
            return 0;
        try { return std::stoi(orderId.substr(prefix.size())); }
        catch (...) { return 0; }
    }
}

OrderNumberGenerator::OrderNumberGenerator(IRepository<Order>& orderRepo)
    : orderRepo_(orderRepo)
{}

std::string OrderNumberGenerator::Generate() {
    std::string prefix = GetTodayPrefix();
    auto all = orderRepo_.LoadAll();
    int maxSeq = 0;
    for (const auto& order : all)
        maxSeq = std::max(maxSeq, ExtractSequence(order.GetId(), prefix));
    char seqBuf[5];
    snprintf(seqBuf, sizeof(seqBuf), "%04d", maxSeq + 1);
    return prefix + seqBuf;
}
