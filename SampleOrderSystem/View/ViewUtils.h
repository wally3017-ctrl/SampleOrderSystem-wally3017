#pragma once
#include <string>
#include <algorithm>

namespace ViewUtils {

    inline std::string Pad(int columnWidth, int usedChars) {
        return std::string(std::max(1, columnWidth - usedChars), ' ');
    }

    inline std::string PadTo(const std::string& text, int columnWidth) {
        return text + Pad(columnWidth, static_cast<int>(text.size()));
    }

}
