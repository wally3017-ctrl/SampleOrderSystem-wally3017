#pragma once
#include "IMenuController.h"
#include "../Production/ProductionQueue.h"
#include "../View/ProductionView.h"

class ProductionController : public IMenuController {
    ProductionQueue& queue_;
    ProductionView&  view_;
public:
    ProductionController(ProductionQueue& queue, ProductionView& view);
    void Run() override;
};
