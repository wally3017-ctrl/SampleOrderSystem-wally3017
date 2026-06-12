#include "SampleController.h"
#include <stdexcept>

SampleController::SampleController(IRepository<Sample>& repo)
    : repo_(repo)
{}

void SampleController::Register(const Sample& sample) {
    if (sample.GetYield() <= 0.0 || sample.GetYield() > 1.0)
        throw std::invalid_argument("수율은 0 초과 1 이하여야 합니다.");
    if (sample.GetAvgProductionTime() <= 0)
        throw std::invalid_argument("평균 생산시간은 양수여야 합니다.");
    if (repo_.Load(sample.GetId()).has_value())
        throw std::invalid_argument("이미 존재하는 시료 ID입니다: " + sample.GetId());
    repo_.Save(sample);
}

void SampleController::Run() {
    // Phase 2-4에서 구현
}
