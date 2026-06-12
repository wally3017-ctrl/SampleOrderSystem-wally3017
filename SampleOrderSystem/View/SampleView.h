#pragma once
#include "ConsoleView.h"
#include "../Model/Sample.h"
#include <string>
#include <vector>

class SampleView {
    ConsoleView& console_;

public:
    explicit SampleView(ConsoleView& console);

    void ShowMenu();
    void ShowList(const std::vector<Sample>& samples);
    void ShowSearchResult(const std::vector<Sample>& samples, const std::string& keyword);
    void ShowRegistered(const Sample& sample);
    void ShowError(const std::string& msg);

    std::string ReadInput();
    std::string ReadId();
    std::string ReadName();
    int         ReadAvgProductionTime();
    double      ReadYield();
    std::string ReadSearchKeyword();
};
