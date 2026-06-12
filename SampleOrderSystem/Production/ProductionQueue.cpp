#include "ProductionQueue.h"
#include <fstream>
#include <sstream>

namespace {
    std::string SerializeArray(const std::deque<ProductionJob>& jobs) {
        std::string out = "[";
        for (size_t i = 0; i < jobs.size(); ++i) {
            if (i > 0) out += ",";
            out += jobs[i].ToJson().Serialize();
        }
        out += "]";
        return out;
    }

    std::vector<ProductionJob> DeserializeArray(const std::string& json) {
        std::vector<ProductionJob> result;
        size_t pos = 0;
        while (pos < json.size() && json[pos] != '[') ++pos;
        if (pos >= json.size()) return result;
        ++pos;
        while (pos < json.size() && json[pos] != ']') {
            while (pos < json.size() && json[pos] != '{' && json[pos] != ']') ++pos;
            if (pos >= json.size() || json[pos] == ']') break;
            size_t start = pos;
            int depth = 0;
            while (pos < json.size()) {
                if      (json[pos] == '{') ++depth;
                else if (json[pos] == '}') { if (--depth == 0) { ++pos; break; } }
                ++pos;
            }
            result.push_back(
                ProductionJob::FromJson(JsonObject::Deserialize(json.substr(start, pos - start))));
        }
        return result;
    }
}

ProductionQueue::ProductionQueue(const std::string& filename)
    : filename_(filename)
{
    if (!filename_.empty()) LoadFromFile();
}

void ProductionQueue::SaveToFile() const {
    if (filename_.empty()) return;
    std::ofstream f(filename_);
    if (f) f << SerializeArray(queue_);
}

void ProductionQueue::LoadFromFile() {
    std::ifstream f(filename_);
    if (!f) return;
    std::ostringstream oss;
    oss << f.rdbuf();
    auto jobs = DeserializeArray(oss.str());
    queue_.assign(jobs.begin(), jobs.end());
}

void ProductionQueue::Enqueue(const ProductionJob& job) {
    queue_.push_back(job);
    SaveToFile();
}

std::optional<ProductionJob> ProductionQueue::Dequeue() {
    if (queue_.empty()) return std::nullopt;
    ProductionJob front = queue_.front();
    queue_.pop_front();
    SaveToFile();
    return front;
}

std::optional<ProductionJob> ProductionQueue::Peek() const {
    if (queue_.empty()) return std::nullopt;
    return queue_.front();
}

std::vector<ProductionJob> ProductionQueue::GetQueue() const {
    return std::vector<ProductionJob>(queue_.begin(), queue_.end());
}

bool ProductionQueue::IsEmpty() const {
    return queue_.empty();
}

std::size_t ProductionQueue::Size() const {
    return queue_.size();
}
