# Phase 2 — 시료 관리 (Sample Management)

**목표**: 시료 등록·조회·검색 기능 완성

---

## 작업 항목

| 항목 | 설명 |
|------|------|
| 시료 모델 | `Sample` — ID, 이름, 평균 생산시간(min/ea), 수율, 재고 수량 |
| 시료 등록 | 시료 ID 중복 검사 후 등록 |
| 시료 목록 조회 | 등록된 전체 시료 + 현재 재고 수량 표시 |
| 시료 검색 | 이름 등 속성으로 부분 검색 |
| 시료 Repository | JSON 기반 CRUD 구현 |

## 도메인 규칙

- 수율 = 정상 시료 수 / 총 생산 시료 수 (0 < 수율 ≤ 1)
- 시료 ID는 시스템 내 유일해야 함
- 등록된 시료만 주문 가능

---

## TDD 세부 Phase

### Phase 2-1 — [Red] 시료 모델 및 등록 테스트 작성

> 시료 도메인 모델과 등록 로직의 테스트를 먼저 작성한다.

- `Sample` 모델 클래스 정의 (ID, 이름, 평균 생산시간, 수율, 재고 수량)
- gmock으로 `MockSampleRepository` 생성
- 아래 테스트 케이스 작성 (모두 실패 상태)
  - `RegisterSample_ValidInput_SavesSuccessfully`
  - `RegisterSample_DuplicateId_ThrowsException`
  - `RegisterSample_YieldZero_ThrowsException`
  - `RegisterSample_YieldOverOne_ThrowsException`
  - `RegisterSample_NegativeProductionTime_ThrowsException`

---

### Phase 2-2 — [Green] 시료 등록 구현

> 테스트를 통과하는 시료 등록 기능을 구현한다.

- `SampleController.Register()` — 중복 ID 검사 후 저장
- `SampleRepository` — Phase 1의 `JsonRepository<Sample>` 활용
- 수율 범위(0 < 수율 ≤ 1) 및 생산시간 양수 검증
- 모든 Phase 2-1 테스트 통과

---

### Phase 2-3 — [Red] 시료 조회 및 검색 테스트 작성

> 목록 조회와 검색 동작의 테스트를 먼저 작성한다.

- 아래 테스트 케이스 작성 (모두 실패 상태)
  - `GetAllSamples_EmptyRepository_ReturnsEmptyList`
  - `GetAllSamples_MultipleSamples_ReturnsAllWithStock`
  - `SearchSample_ExactName_ReturnsMatch`
  - `SearchSample_PartialName_ReturnsMatches`
  - `SearchSample_NoMatch_ReturnsEmptyList`

---

### Phase 2-4 — [Green] 시료 조회 및 검색 구현

> 테스트를 통과하는 조회·검색 기능을 구현한다.

- `SampleController.GetAll()` — 전체 목록 + 현재 재고 수량 반환
- `SampleController.Search(keyword)` — 이름 기준 부분 일치 검색
- `SampleView` — 목록 및 검색 결과 화면 출력
- 모든 Phase 2-3 테스트 통과

---

### Phase 2-5 — [Refactor] 리팩토링

> 동작은 유지하면서 코드 품질을 개선한다.

- `Sample` 모델 유효성 검증 로직 분리 검토
- 검색 조건 확장 가능 구조 검토
- 매직 문자열·숫자 상수화
- 전체 Phase 2 테스트 재실행하여 통과 확인
