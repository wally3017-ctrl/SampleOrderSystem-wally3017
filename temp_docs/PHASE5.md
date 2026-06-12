# Phase 5 — 모니터링 및 출고 처리

**목표**: 모니터링 대시보드, 출고 처리 기능 완성 및 전체 흐름 통합

---

## 작업 항목

| 항목 | 설명 |
|------|------|
| 모니터링 — 주문량 | 상태별(RESERVED / CONFIRMED / PRODUCING / RELEASED) 주문 수 표시 (REJECTED 제외) |
| 모니터링 — 재고량 | 시료별 재고 수량 + 상태 표기 (여유 / 부족 / 고갈) |
| 재고 상태 판단 | 여유: 주문 대비 재고 충분 / 부족: 재고 부족 / 고갈: 재고 0 |
| 출고 처리 | CONFIRMED 주문 목록 조회 → 선택 → **RELEASED** 전환 |
| 전체 흐름 통합 테스트 | Phase 1~4 연계 시나리오 검증 |

## 도메인 규칙

- 모니터링에서 REJECTED 주문은 완전 제외
- 출고는 CONFIRMED 상태 주문에 대해서만 실행 가능
- 출고 완료 시 상태: CONFIRMED → **RELEASED**

---

## TDD 세부 Phase

### Phase 5-1 — [Red] 모니터링 집계 테스트 작성

> 주문량 집계와 재고 상태 분류 동작의 테스트를 먼저 작성한다.

- `MonitoringService` 인터페이스 정의
- `StockStatus` 열거형 정의 (여유 / 부족 / 고갈)
- gmock으로 `MockOrderRepository`, `MockSampleRepository` 생성
- 아래 테스트 케이스 작성 (모두 실패 상태)
  - `GetOrderSummary_ExcludesRejected_ReturnsCorrectCounts`
  - `GetOrderSummary_AllStatuses_CountsIndependently`
  - `GetStockStatus_StockIsZero_ReturnsGodal`
  - `GetStockStatus_StockLessThanOrdered_ReturnsBujok`
  - `GetStockStatus_StockSufficient_ReturnsYeoyoo`

---

### Phase 5-2 — [Green] 모니터링 구현

> 테스트를 통과하는 모니터링 집계 로직을 구현한다.

- `MonitoringService.GetOrderSummary()` — 상태별 주문 수 집계 (REJECTED 제외)
- `MonitoringService.GetStockStatus()` — 시료별 재고 상태 분류
- `MonitoringView` — 주문량 및 재고량 현황 화면 출력
- 모든 Phase 5-1 테스트 통과

---

### Phase 5-3 — [Red] 출고 처리 테스트 작성

> 출고 처리 흐름의 테스트를 먼저 작성한다.

- 아래 테스트 케이스 작성 (모두 실패 상태)
  - `Release_ConfirmedOrder_ChangesToReleased`
  - `Release_NonConfirmedOrder_ThrowsException`
  - `GetReleasableOrders_ReturnsOnlyConfirmed`
  - `GetReleasableOrders_NoConfirmedOrders_ReturnsEmptyList`

---

### Phase 5-4 — [Green] 출고 처리 구현

> 테스트를 통과하는 출고 처리 기능을 구현한다.

- `ReleaseController.Release()` — CONFIRMED → RELEASED 상태 전환
- `ReleaseController.GetReleasableOrders()` — CONFIRMED 상태 주문 목록 반환
- `ReleaseView` — 출고 가능 목록 및 처리 결과 화면 출력
- 모든 Phase 5-3 테스트 통과

---

### Phase 5-5 — [Red] 통합 시나리오 테스트 작성

> Phase 1~4 전체 흐름을 연결하는 통합 테스트를 먼저 작성한다.

- 아래 통합 시나리오 테스트 케이스 작성 (모두 실패 상태)
  - `Scenario_OrderToRelease_SufficientStock_FullFlow`
    - 시료 등록 → 주문 접수 → 승인(재고 충분) → 출고 처리
  - `Scenario_OrderToRelease_InsufficientStock_FullFlow`
    - 시료 등록 → 주문 접수 → 승인(재고 부족) → 생산 완료 → 출고 처리
  - `Scenario_RejectOrder_ExcludedFromMonitoring`
    - 주문 접수 → 거절 → 모니터링 집계에서 제외 확인
  - `Scenario_MultipleOrders_FIFOProductionOrder`
    - 여러 주문 접수(재고 부족) → 생산라인 FIFO 순서 처리 확인

---

### Phase 5-6 — [Green] 통합 시나리오 통과

> 모든 통합 테스트를 통과하도록 Phase 간 연결을 검증하고 수정한다.

- Controller 간 의존성 최종 점검
- 데이터 영속성 연동 최종 확인 (재시작 후 상태 유지)
- 모든 Phase 5-5 통합 테스트 통과

---

### Phase 5-7 — [Refactor] 최종 리팩토링

> 전체 코드베이스를 대상으로 최종 품질 개선을 수행한다.

- 전체 Phase(1~5)에 걸친 중복 로직 통합 검토
- `StockStatus` 열거형 활용 일관성 점검
- 불필요한 의존성 제거
- **전체 테스트 스위트 재실행하여 모두 통과 확인**
