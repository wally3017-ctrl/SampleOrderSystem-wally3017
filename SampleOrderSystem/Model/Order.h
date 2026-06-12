#pragma once
#include "OrderStatus.h"
#include "../Utility/JsonObject.h"
#include <string>

class Order {
    std::string orderId_;
    std::string sampleId_;
    std::string customerName_;
    int         quantity_;
    OrderStatus status_;
    std::string orderedAt_;

public:
    Order() = default;
    Order(std::string orderId, std::string sampleId, std::string customerName,
          int quantity, OrderStatus status, std::string orderedAt)
        : orderId_(std::move(orderId))
        , sampleId_(std::move(sampleId))
        , customerName_(std::move(customerName))
        , quantity_(quantity)
        , status_(status)
        , orderedAt_(std::move(orderedAt))
    {}

    std::string GetId()           const { return orderId_; }
    std::string GetSampleId()     const { return sampleId_; }
    std::string GetCustomerName() const { return customerName_; }
    int         GetQuantity()     const { return quantity_; }
    OrderStatus GetStatus()       const { return status_; }
    std::string GetOrderedAt()    const { return orderedAt_; }

    void SetStatus(OrderStatus status) { status_ = status; }

    bool operator==(const Order& o) const { return orderId_ == o.orderId_; }

    JsonObject ToJson() const {
        JsonObject j;
        j.Set("orderId",      orderId_);
        j.Set("sampleId",     sampleId_);
        j.Set("customerName", customerName_);
        j.Set("quantity",     quantity_);
        j.Set("status",       ToString(status_));
        j.Set("orderedAt",    orderedAt_);
        return j;
    }

    static Order FromJson(const JsonObject& j) {
        return Order{
            j.GetString("orderId"),
            j.GetString("sampleId"),
            j.GetString("customerName"),
            j.GetInt("quantity"),
            OrderStatusFromString(j.GetString("status")),
            j.GetString("orderedAt")
        };
    }
};
