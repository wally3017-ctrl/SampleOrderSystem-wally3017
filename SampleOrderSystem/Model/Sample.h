#pragma once
#include <string>
#include "../Utility/JsonObject.h"

class Sample {
    std::string id_;
    std::string name_;
    int    avgProductionTime_;
    double yield_;
    int    stock_;

public:
    Sample() = default;
    Sample(std::string id, std::string name, int avgProductionTime, double yield, int stock = 0)
        : id_(std::move(id))
        , name_(std::move(name))
        , avgProductionTime_(avgProductionTime)
        , yield_(yield)
        , stock_(stock)
    {}

    std::string GetId()                const { return id_; }
    std::string GetName()              const { return name_; }
    int         GetAvgProductionTime() const { return avgProductionTime_; }
    double      GetYield()             const { return yield_; }
    int         GetStock()             const { return stock_; }

    void SetStock(int stock) { stock_ = stock; }

    bool operator==(const Sample& o) const {
        return id_ == o.id_ && name_ == o.name_
            && avgProductionTime_ == o.avgProductionTime_
            && yield_ == o.yield_ && stock_ == o.stock_;
    }

    JsonObject ToJson() const {
        JsonObject j;
        j.Set("id",   id_);
        j.Set("name", name_);
        j.Set("avgProductionTime", avgProductionTime_);
        j.Set("yield", yield_);
        j.Set("stock", stock_);
        return j;
    }

    static Sample FromJson(const JsonObject& j) {
        return Sample{
            j.GetString("id"),
            j.GetString("name"),
            j.GetInt("avgProductionTime"),
            j.GetDouble("yield"),
            j.GetInt("stock")
        };
    }
};
