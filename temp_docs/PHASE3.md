# Phase 3 — 주문 관리 (Order Management)

**목표**: 주문 접수 → 승인/거절 → 상태 전이 로직 완성

---

## 작업 항목

| 항목 | 설명 |
|------|------|
| 주문 모델 | `Order` — 주문번호, 시료 ID, 고객명, 주문 수량, 상태, 접수 일시 |
| 주문 번호 생성 | `ORD-YYYYMMDD-NNNN` 형식 자동 생성 |
| 시료 주문 (접수) | 시료 ID / 고객명 / 주문 수량 입력 → 상태 **RESERVED** |
| 주문 승인 | RESERVED → 재고 확인 → **CONFIRMED** 또는 **PRODUCING** 자동 분기 |
| 주문 거절 | RESERVED → **REJECTED** 즉시 전환 |
| 주문 Repository | JSON 기반 CRUD, 상태별 조회 |

## 도메인 규칙

- 승인 시 재고 ≥ 주문 수량 → 즉시 **CONFIRMED**
- 승인 시 재고 < 주문 수량 → 생산라인 자동 등록 + **PRODUCING**
- 거절된 주문(REJECTED)은 이후 모든 흐름에서 제외

---

## TDD 세부 Phase

### Phase 3-1 — [Red] 주문 접수 테스트 작성

> 주문 접수 흐름의 테스트를 먼저 작성한다.

- `Order` 모델 클래스 정의 (주문번호, 시료 ID, 고객명, 주문 수량, 상태, 접수 일시)
- `OrderStatus` 열거형 정의 (RESERVED / REJECTED / PRODUCING / CONFIRMED / RELEASED)
- gmock으로 `MockOrderRepository`, `MockSampleRepository` 생성
- 아래 테스트 케이스 작성 (모두 실패 상태)
  - `PlaceOrder_ValidSampleId_CreatesReservedOrder`
  - `PlaceOrder_InvalidSampleId_ThrowsException`
  - `PlaceOrder_ZeroQuantity_ThrowsException`
  - `GenerateOrderNumber_Format_MatchesOrdYYYYMMDDNNNN`
  - `GenerateOrderNumber_MultipleOrders_NoDuplicate`

---

### Phase 3-2 — [Green] 주문 접수 구현

> 테스트를 통과하는 주문 접수 기능을 구현한다.

- `OrderController.PlaceOrder()` — 시료 존재 검증 후 RESERVED 상태로 저장
- `OrderNumberGenerator` — `ORD-YYYYMMDD-NNNN` 형식, 당일 순번 중복 방지
- `OrderRepository` — Phase 1의 `JsonRepository<Order>` 활용
- 모든 Phase 3-1 테스트 통과

---

### Phase 3-3 — [Red] 주문 승인/거절 테스트 작성

> 재고 분기 및 상태 전이 동작의 테스트를 먼저 작성한다.

- 아래 테스트 케이스 작성 (모두 실패 상태)
  - `ApproveOrder_SufficientStock_ChangesToConfirmed`
  - `ApproveOrder_InsufficientStock_ChangesToProducing`
  - `ApproveOrder_InsufficientStock_RegistersProductionJob`
  - `ApproveOrder_NonReservedOrder_ThrowsException`
  - `RejectOrder_ReservedOrder_ChangesToRejected`
  - `RejectOrder_NonReservedOrder_ThrowsException`

---

### Phase 3-4 — [Green] 주문 승인/거절 구현

> 테스트를 통과하는 승인/거절 로직을 구현한다.

- `OrderController.Approve()` — 재고 확인 후 CONFIRMED 또는 PRODUCING 분기
- `OrderController.Reject()` — 즉시 REJECTED 전환
- PRODUCING 전환 시 `ProductionQueue`에 자동 등록 (Phase 4 인터페이스 활용)
- `OrderView` — RESERVED 목록 및 처리 결과 화면 출력
- 모든 Phase 3-3 테스트 통과

---

### Phase 3-5 — [Refactor] 리팩토링

> 동작은 유지하면서 코드 품질을 개선한다.

- 상태 전이 로직을 `OrderStateMachine` 또는 전략 패턴으로 분리 검토
- 재고 판단 조건 명확한 메서드로 추출
- `OrderStatus` 열거형 활용 일관성 점검
- 전체 Phase 3 테스트 재실행하여 통과 확인
