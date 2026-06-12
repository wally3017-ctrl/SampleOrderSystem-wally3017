#pragma once
#include <map>
#include <stdexcept>
#include <string>
#include <sstream>

class JsonObject {
public:
    void Set(const std::string& key, std::string value) {
        fields_[key] = std::move(value);
    }

    void Set(const std::string& key, int value) {
        fields_[key] = std::to_string(value);
    }

    void Set(const std::string& key, double value) {
        fields_[key] = std::to_string(value);
    }

    std::string GetString(const std::string& key) const {
        return fields_.at(key);
    }

    int GetInt(const std::string& key) const {
        return std::stoi(fields_.at(key));
    }

    double GetDouble(const std::string& key) const {
        return std::stod(fields_.at(key));
    }

    // {"key":"value","key2":"value2"} 형태로 직렬화
    std::string Serialize() const {
        std::ostringstream oss;
        oss << "{";
        bool first = true;
        for (const auto& [key, value] : fields_) {
            if (!first) oss << ",";
            oss << "\"" << Escape(key) << "\":\"" << Escape(value) << "\"";
            first = false;
        }
        oss << "}";
        return oss.str();
    }

    static JsonObject Deserialize(const std::string& json) {
        JsonObject obj;
        size_t pos = 0;
        SkipTo(json, pos, '{');
        ++pos;
        while (pos < json.size() && json[pos] != '}') {
            SkipTo(json, pos, '"');
            if (pos >= json.size()) break;
            auto key = ParseString(json, pos);
            SkipTo(json, pos, ':');
            ++pos;
            SkipTo(json, pos, '"');
            if (pos >= json.size()) break;
            auto value = ParseString(json, pos);
            obj.fields_[key] = value;
            while (pos < json.size() && json[pos] != ',' && json[pos] != '}') ++pos;
            if (pos < json.size() && json[pos] == ',') ++pos;
        }
        return obj;
    }

private:
    std::map<std::string, std::string> fields_;

    static void SkipTo(const std::string& s, size_t& pos, char target) {
        while (pos < s.size() && s[pos] != target) ++pos;
    }

    static std::string Escape(const std::string& s) {
        std::string out;
        out.reserve(s.size());
        for (char c : s) {
            if      (c == '"')  out += "\\\"";
            else if (c == '\\') out += "\\\\";
            else if (c == '\n') out += "\\n";
            else if (c == '\r') out += "\\r";
            else                out += c;
        }
        return out;
    }

    static std::string ParseString(const std::string& s, size_t& pos) {
        ++pos; // opening "
        std::string result;
        while (pos < s.size() && s[pos] != '"') {
            if (s[pos] == '\\' && pos + 1 < s.size()) {
                ++pos;
                switch (s[pos]) {
                case '"':  result += '"';  break;
                case '\\': result += '\\'; break;
                case 'n':  result += '\n'; break;
                case 'r':  result += '\r'; break;
                default:   result += s[pos]; break;
                }
            } else {
                result += s[pos];
            }
            ++pos;
        }
        ++pos; // closing "
        return result;
    }
};
