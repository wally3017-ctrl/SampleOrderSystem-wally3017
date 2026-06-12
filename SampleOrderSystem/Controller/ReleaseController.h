#pragma once
#include "IMenuController.h"
#include "../Model/Order.h"
#include "../Repository/IRepository.h"
#include "../View/ReleaseView.h"
#include <vector>
#include <string>

class ReleaseController : public IMenuController {
    IRepository<Order>& orderRepo_;
    ReleaseView&        view_;
public:
    ReleaseController(IRepository<Order>& orderRepo, ReleaseView& view);
    void               Release(const std::string& orderId);
    std::vector<Order> GetReleasableOrders() const;
    void               Run() override;
};
