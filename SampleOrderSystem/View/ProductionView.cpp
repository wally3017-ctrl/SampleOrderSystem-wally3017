#include "ProductionView.h"

namespace {
    constexpr int COL_ORDERID = 22;
    constexpr int COL_SAMPLEID = 8;

    std::string pad(int width, int used) {
        return std::string(std::max(1, width - used), ' ');
    }

    void PrintJobRow(ConsoleView& console, const ProductionJob& job, bool isCurrent) {
        std::string prefix = isCurrent ? " >> " : "    ";
        console.PrintLine(prefix
            + job.GetOrderId()
            + pad(COL_ORDERID, (int)job.GetOrderId().size())
            + job.GetSampleId()
            + pad(COL_SAMPLEID, (int)job.GetSampleId().size())
            + "부족:" + std::to_string(job.GetShortage())
            + "  생산:" + std::to_string(job.GetActualQty())
            + "ea  예상:" + std::to_string(job.GetTotalTime()) + "min");
    }
}

ProductionView::ProductionView(ConsoleView& console) : console_(console) {}

void ProductionView::ShowQueue(const std::optional<ProductionJob>& current,
                               const std::vector<ProductionJob>&    waiting) {
    console_.PrintSeparator();
    console_.PrintLine("                생산라인 현황");
    console_.PrintSeparator();
    console_.PrintLine(" 주문번호              시료ID  부족/생산량/예상시간");
    console_.PrintLine(" " + std::string(54, '-'));

    if (!current.has_value() && waiting.empty()) {
        console_.PrintLine(" 현재 생산 중인 작업이 없습니다.");
    } else {
        if (current.has_value())
            PrintJobRow(console_, current.value(), true);
        for (const auto& job : waiting)
            PrintJobRow(console_, job, false);
    }
    console_.PrintSeparator();
    console_.PrintLine(" 총 대기 작업: " + std::to_string(waiting.size()) + "건");
    console_.PrintSeparator();
}

void ProductionView::ShowProductionMenu() {
    console_.PrintLine(" 1. 생산 완료 처리    0. 메인 메뉴로 돌아가기");
    console_.Print(" 선택 >> ");
}

void ProductionView::ShowCompleted(const ProductionJob& job) {
    console_.PrintLine(" [생산 완료] " + job.GetOrderId()
        + " (시료:" + job.GetSampleId()
        + "  실 생산량:" + std::to_string(job.GetActualQty()) + "ea)");
    console_.PrintLine(" 해당 주문이 CONFIRMED 상태로 전환되었습니다.");
}

void ProductionView::ShowEmptyQueue() {
    console_.PrintLine(" 현재 생산 대기 중인 작업이 없습니다.");
}

std::string ProductionView::ReadInput() {
    return console_.ReadLine();
}
