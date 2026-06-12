#include "OrderNumberGenerator.h"
#include <ctime>
#include <cstdio>
#include <algorithm>

OrderNumberGenerator::OrderNumberGenerator(IRepository<Order>& orderRepo)
    : orderRepo_(orderRepo)
{}

std::string OrderNumberGenerator::Generate() {
    time_t t = time(nullptr);
    tm tm_info{};
    localtime_s(&tm_info, &t);

    char dateBuf[9];
    strftime(dateBuf, sizeof(dateBuf), "%Y%m%d", &tm_info);
    std::string dateStr(dateBuf);
    std::string prefix = "ORD-" + dateStr + "-";

    auto all = orderRepo_.LoadAll();
    int maxSeq = 0;
    for (const auto& order : all) {
        const auto& id = order.GetId();
        if (id.size() > prefix.size() && id.substr(0, prefix.size()) == prefix) {
            try {
                int seq = std::stoi(id.substr(prefix.size()));
                maxSeq = std::max(maxSeq, seq);
            } catch (...) {}
        }
    }

    char seqBuf[5];
    snprintf(seqBuf, sizeof(seqBuf), "%04d", maxSeq + 1);
    return prefix + seqBuf;
}
