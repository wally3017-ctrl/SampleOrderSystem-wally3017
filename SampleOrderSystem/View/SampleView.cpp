#include "SampleView.h"
#include <stdexcept>

SampleView::SampleView(ConsoleView& console) : console_(console) {}

void SampleView::ShowMenu() {
    console_.PrintSeparator();
    console_.PrintLine("                  시료 관리");
    console_.PrintSeparator();
    console_.PrintLine(" 1. 시료 등록");
    console_.PrintLine(" 2. 시료 목록 조회");
    console_.PrintLine(" 3. 시료 검색");
    console_.PrintLine(" 0. 메인 메뉴로 돌아가기");
    console_.PrintSeparator();
    console_.Print(" 메뉴 선택 >> ");
}

void SampleView::ShowList(const std::vector<Sample>& samples) {
    console_.PrintSeparator();
    console_.PrintLine(" [시료 목록]");
    console_.PrintSeparator();
    if (samples.empty()) {
        console_.PrintLine(" 등록된 시료가 없습니다.");
    } else {
        console_.PrintLine(" ID        이름              생산시간  수율    재고");
        console_.PrintLine(" " + std::string(57, '-'));
        for (const auto& s : samples) {
            std::string row =
                " " + s.GetId() +
                std::string(std::max(1, 10 - (int)s.GetId().size()), ' ') +
                s.GetName() +
                std::string(std::max(1, 18 - (int)s.GetName().size()), ' ') +
                std::to_string(s.GetAvgProductionTime()) + "min" +
                std::string(std::max(1, 6 - (int)std::to_string(s.GetAvgProductionTime()).size()), ' ') +
                std::to_string(static_cast<int>(s.GetYield() * 100)) + "%" +
                std::string(4, ' ') +
                std::to_string(s.GetStock()) + "개";
            console_.PrintLine(row);
        }
    }
    console_.PrintSeparator();
}

void SampleView::ShowSearchResult(const std::vector<Sample>& samples, const std::string& keyword) {
    console_.PrintSeparator();
    console_.PrintLine(" [검색 결과] 키워드: " + keyword);
    console_.PrintSeparator();
    if (samples.empty()) {
        console_.PrintLine(" 검색 결과가 없습니다.");
    } else {
        ShowList(samples);
        return;
    }
    console_.PrintSeparator();
}

void SampleView::ShowRegistered(const Sample& sample) {
    console_.PrintLine(" 시료가 등록되었습니다: [" + sample.GetId() + "] " + sample.GetName());
}

void SampleView::ShowError(const std::string& msg) {
    console_.PrintError(msg);
}

std::string SampleView::ReadInput() {
    return console_.ReadLine();
}

std::string SampleView::ReadId() {
    console_.Print(" 시료 ID     : ");
    return console_.ReadLine();
}

std::string SampleView::ReadName() {
    console_.Print(" 시료 이름   : ");
    return console_.ReadLine();
}

int SampleView::ReadAvgProductionTime() {
    console_.Print(" 평균 생산시간(min/ea): ");
    std::string line = console_.ReadLine();
    try { return std::stoi(line); }
    catch (...) { throw std::invalid_argument("숫자를 입력해야 합니다."); }
}

double SampleView::ReadYield() {
    console_.Print(" 수율 (0 초과 1 이하): ");
    std::string line = console_.ReadLine();
    try { return std::stod(line); }
    catch (...) { throw std::invalid_argument("숫자를 입력해야 합니다."); }
}

std::string SampleView::ReadSearchKeyword() {
    console_.Print(" 검색어 : ");
    return console_.ReadLine();
}
