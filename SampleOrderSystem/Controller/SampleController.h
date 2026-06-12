#pragma once
#include "IMenuController.h"
#include "../Model/Sample.h"
#include "../Repository/IRepository.h"
#include "../View/SampleView.h"
#include <string>
#include <vector>

class SampleController : public IMenuController {
    IRepository<Sample>& repo_;
    SampleView&          view_;

public:
    SampleController(IRepository<Sample>& repo, SampleView& view);

    void Register(const Sample& sample);
    std::vector<Sample> GetAll() const;
    std::vector<Sample> Search(const std::string& keyword) const;

    void Run() override;
};
