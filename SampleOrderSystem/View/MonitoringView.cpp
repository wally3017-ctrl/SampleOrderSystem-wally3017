#include "MonitoringView.h"
#include "ViewUtils.h"

MonitoringView::MonitoringView(ConsoleView& console) : console_(console) {}

void MonitoringView::ShowMonitoring(const OrderSummary& summary,
                                    const std::vector<SampleStockInfo>& stockInfos) {
    console_.PrintSeparator();
    console_.PrintLine("                   모니터링 현황");
    console_.PrintSeparator();

    console_.PrintLine(" [주문량]");
    console_.PrintLine("  RESERVED : " + std::to_string(summary.reserved)  + " 건");
    console_.PrintLine("  PRODUCING: " + std::to_string(summary.producing) + " 건");
    console_.PrintLine("  CONFIRMED: " + std::to_string(summary.confirmed) + " 건");
    console_.PrintLine("  RELEASED : " + std::to_string(summary.released)  + " 건");
    console_.PrintSeparator();

    console_.PrintLine(" [재고량]");
    console_.PrintLine(" 시료ID      시료명              재고   주문대기  상태");
    console_.PrintLine(" " + std::string(54, '-'));
    for (const auto& info : stockInfos) {
        console_.PrintLine(" "
            + ViewUtils::PadTo(info.sampleId,   12)
            + ViewUtils::PadTo(info.sampleName, 20)
            + ViewUtils::PadTo(std::to_string(info.stock),      7)
            + ViewUtils::PadTo(std::to_string(info.orderedQty), 9)
            + "[" + ToString(info.status) + "]");
    }
    if (stockInfos.empty())
        console_.PrintLine(" 등록된 시료가 없습니다.");
    console_.PrintSeparator();
    console_.PrintLine(" 0. 메인 메뉴로 돌아가기");
    console_.PrintSeparator();
    console_.Print(" 선택 >> ");
}

std::string MonitoringView::ReadInput() {
    return console_.ReadLine();
}
