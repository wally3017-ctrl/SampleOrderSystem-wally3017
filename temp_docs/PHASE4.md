# Phase 4 — 생산라인 (Production Line)

**목표**: FIFO 생산 큐, 실 생산량 계산, 생산 완료 후 상태 전이 구현

---

## 작업 항목

| 항목 | 설명 |
|------|------|
| 생산 작업 모델 | `ProductionJob` — 주문 ID, 시료 ID, 부족분, 실 생산량, 총 생산 시간, 등록 일시 |
| FIFO 생산 큐 | 선입선출 대기열 관리 |
| 실 생산량 계산 | `ceil(부족분 / (수율 × 0.9))` |
| 총 생산 시간 계산 | `평균 생산시간 × 실 생산량` |
| 생산 완료 처리 | PRODUCING → **CONFIRMED** 자동 전환, 재고 수량 증가 |
| 생산라인 현황 화면 | 현재 처리 중인 주문 정보, 대기 큐 목록 표시 |

## 도메인 규칙

- 단일 생산라인 — 동시에 하나의 작업만 처리
- 스케줄링 전략: **FIFO** (선입선출)
- 생산 완료 시 해당 주문 상태 PRODUCING → CONFIRMED로 전환

---

## TDD 세부 Phase

### Phase 4-1 — [Red] 생산량 계산 테스트 작성

> 핵심 계산 로직의 테스트를 먼저 작성한다.

- `ProductionCalculator` 클래스 인터페이스 정의
- 아래 테스트 케이스 작성 (모두 실패 상태)
  - `CalcActualProduction_NormalYield_ReturnsCorrectCeil`
  - `CalcActualProduction_HighYield_ReturnsMinimumNeeded`
  - `CalcActualProduction_LowYield_ReturnsLargerQuantity`
  - `CalcActualProduction_ShortageIsZero_ReturnsZero`
  - `CalcTotalProductionTime_ReturnsAvgTimeMultipliedByActual`

  > 예시: 부족분=170, 수율=0.92 → `ceil(170 / (0.92 × 0.9))` = `ceil(170 / 0.828)` = `ceil(205.3)` = 206

---

### Phase 4-2 — [Green] 생산량 계산 구현

> 테스트를 통과하는 계산 로직을 구현한다.

- `ProductionCalculator.CalcActualProduction(shortage, yield)` 구현
- `ProductionCalculator.CalcTotalTime(avgTime, actualQty)` 구현
- 모든 Phase 4-1 테스트 통과

---

### Phase 4-3 — [Red] FIFO 생산 큐 테스트 작성

> 생산 큐의 순서 보장과 등록/조회 동작의 테스트를 먼저 작성한다.

- `ProductionJob` 모델 클래스 정의
- gmock으로 `MockProductionQueue` 생성
- 아래 테스트 케이스 작성 (모두 실패 상태)
  - `Enqueue_MultipleJobs_MaintainsFIFOOrder`
  - `Dequeue_EmptyQueue_ReturnsNull`
  - `Peek_NonEmptyQueue_ReturnsFirstWithoutRemoving`
  - `GetQueue_ReturnsAllJobsInOrder`

---

### Phase 4-4 — [Green] FIFO 생산 큐 구현

> 테스트를 통과하는 생산 큐를 구현한다.

- `ProductionQueue` — FIFO 기반 대기열, JSON 영속성
- `ProductionJob` 등록/조회/완료 처리
- 모든 Phase 4-3 테스트 통과

---

### Phase 4-5 — [Red] 생산 완료 및 상태 전이 테스트 작성

> 생산 완료 시 주문 상태 전이와 재고 반영의 테스트를 먼저 작성한다.

- 아래 테스트 케이스 작성 (모두 실패 상태)
  - `CompleteProduction_ChangesOrderToConfirmed`
  - `CompleteProduction_IncreasesStockByActualProduction`
  - `CompleteProduction_RemovesJobFromQueue`
  - `CompleteProduction_EmptyQueue_NoAction`

---

### Phase 4-6 — [Green] 생산 완료 처리 및 현황 화면 구현

> 테스트를 통과하는 생산 완료 처리와 현황 화면을 구현한다.

- `ProductionController.Complete()` — 생산 완료 처리, PRODUCING → CONFIRMED 전환, 재고 증가
- `ProductionView` — 현재 처리 중인 주문 정보 및 대기 큐 목록 출력
- 모든 Phase 4-5 테스트 통과

---

### Phase 4-7 — [Refactor] 리팩토링

> 동작은 유지하면서 코드 품질을 개선한다.

- `ProductionCalculator` 순수 함수 형태 유지 확인 (부수효과 없음)
- 큐와 완료 처리 책임 분리 검토
- 계산식 상수(`0.9`) 명명 상수로 추출
- 전체 Phase 4 테스트 재실행하여 통과 확인
