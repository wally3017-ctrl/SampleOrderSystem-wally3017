#pragma once
#include <string>
#include <ctime>
#include <cstdio>

namespace Timestamp {

    inline std::string Now() {
        time_t t = time(nullptr);
        tm tm_info{};
        localtime_s(&tm_info, &t);
        char buf[20];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm_info);
        return buf;
    }

    inline time_t Parse(const std::string& ts) {
        tm tm_info{};
        sscanf_s(ts.c_str(), "%d-%d-%d %d:%d:%d",
                 &tm_info.tm_year, &tm_info.tm_mon,  &tm_info.tm_mday,
                 &tm_info.tm_hour, &tm_info.tm_min,  &tm_info.tm_sec);
        tm_info.tm_year -= 1900;
        tm_info.tm_mon  -= 1;
        tm_info.tm_isdst = -1;
        return mktime(&tm_info);
    }

    inline bool IsElapsed(const std::string& ts, int minutes) {
        time_t completion = Parse(ts) + static_cast<time_t>(minutes) * 60;
        return time(nullptr) >= completion;
    }

}
