#include "SampleController.h"
#include "MenuKey.h"
#include <stdexcept>

namespace {
    constexpr std::string_view MENU_REGISTER = "1";
    constexpr std::string_view MENU_LIST     = "2";
    constexpr std::string_view MENU_SEARCH   = "3";
}

SampleController::SampleController(IRepository<Sample>& repo, SampleView& view)
    : repo_(repo), view_(view)
{}

void SampleController::Register(const Sample& sample) {
    sample.Validate();
    if (repo_.Load(sample.GetId()).has_value())
        throw std::invalid_argument("이미 존재하는 시료 ID입니다: " + sample.GetId());
    repo_.Save(sample);
}

std::vector<Sample> SampleController::GetAll() const {
    return repo_.LoadAll();
}

std::vector<Sample> SampleController::Search(const std::string& keyword) const {
    auto all = repo_.LoadAll();
    std::vector<Sample> result;
    for (const auto& s : all) {
        if (s.GetName().find(keyword) != std::string::npos)
            result.push_back(s);
    }
    return result;
}

void SampleController::Run() {
    std::string input;
    while (true) {
        view_.ShowMenu();
        input = view_.ReadInput();
        if (input == MenuKey::EXIT) break;

        if (input == MENU_REGISTER) {
            try {
                std::string id   = view_.ReadId();
                std::string name = view_.ReadName();
                int    time  = view_.ReadAvgProductionTime();
                double yield = view_.ReadYield();
                Sample s(id, name, time, yield);
                Register(s);
                view_.ShowRegistered(s);
            } catch (const std::invalid_argument& e) {
                view_.ShowError(e.what());
            }
        } else if (input == MENU_LIST) {
            view_.ShowList(GetAll());
        } else if (input == MENU_SEARCH) {
            std::string keyword = view_.ReadSearchKeyword();
            view_.ShowSearchResult(Search(keyword), keyword);
        }
    }
}
