#pragma once
#include "../Utility/JsonObject.h"
#include <string>

class ProductionJob {
    std::string orderId_;
    std::string sampleId_;
    int         shortage_;
    int         actualQty_;
    int         totalTime_;
    std::string enqueuedAt_;

public:
    ProductionJob() = default;
    ProductionJob(std::string orderId, std::string sampleId,
                  int shortage, int actualQty, int totalTime, std::string enqueuedAt)
        : orderId_(std::move(orderId))
        , sampleId_(std::move(sampleId))
        , shortage_(shortage)
        , actualQty_(actualQty)
        , totalTime_(totalTime)
        , enqueuedAt_(std::move(enqueuedAt))
    {}

    std::string GetOrderId()    const { return orderId_; }
    std::string GetSampleId()   const { return sampleId_; }
    int         GetShortage()   const { return shortage_; }
    int         GetActualQty()  const { return actualQty_; }
    int         GetTotalTime()  const { return totalTime_; }
    std::string GetEnqueuedAt() const { return enqueuedAt_; }

    bool operator==(const ProductionJob& o) const { return orderId_ == o.orderId_; }

    JsonObject ToJson() const {
        JsonObject j;
        j.Set("orderId",     orderId_);
        j.Set("sampleId",    sampleId_);
        j.Set("shortage",    shortage_);
        j.Set("actualQty",   actualQty_);
        j.Set("totalTime",   totalTime_);
        j.Set("enqueuedAt",  enqueuedAt_);
        return j;
    }

    static ProductionJob FromJson(const JsonObject& j) {
        return ProductionJob{
            j.GetString("orderId"),
            j.GetString("sampleId"),
            j.GetInt("shortage"),
            j.GetInt("actualQty"),
            j.GetInt("totalTime"),
            j.GetString("enqueuedAt")
        };
    }
};
