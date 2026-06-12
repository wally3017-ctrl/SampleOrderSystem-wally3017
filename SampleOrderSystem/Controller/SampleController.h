#pragma once
#include "IMenuController.h"
#include "../Model/Sample.h"
#include "../Repository/IRepository.h"
#include <string>
#include <vector>

class SampleController : public IMenuController {
    IRepository<Sample>& repo_;

public:
    explicit SampleController(IRepository<Sample>& repo);

    void Register(const Sample& sample);
    std::vector<Sample> GetAll() const;
    std::vector<Sample> Search(const std::string& keyword) const;

    void Run() override;
};
