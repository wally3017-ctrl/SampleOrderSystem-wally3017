# Phase 1 — 프로젝트 기반 구축

**목표**: MVC 스켈레톤, 데이터 영속성, 콘솔 입출력 기반 마련

---

## 작업 항목

| 항목 | 설명 |
|------|------|
| 프로젝트 구조 생성 | `Model` / `Controller` / `View` / `Repository` 패키지 분리 |
| 메인 진입점 | `Program.cs` — 메인 루프, 메뉴 라우팅 |
| 콘솔 I/O 유틸 | 입력 처리, 화면 출력 공통 헬퍼 |
| 데이터 영속성 | JSON 파일 기반 저장·불러오기 구현, CRUD 공통 인터페이스 정의 |
| 메인 메뉴 화면 | 시스템 현황 요약(등록 시료 수, 총 재고, 전체 주문 수, 생산라인 대기 수) 표시 |

## 산출물

- `Model/` — 도메인 모델 기반 클래스
- `Repository/` — JSON 기반 데이터 저장소
- `View/MainMenuView` — 메인 메뉴 화면
- `Controller/MainController` — 메뉴 라우팅

---

## TDD 세부 Phase

### Phase 1-1 — [Red] Repository 인터페이스 및 테스트 작성

> 구현 없이 테스트를 먼저 작성하여 실패 상태를 확인한다.

- `IRepository<T>` 인터페이스 정의 (Save / Load / Update / Delete)
- gmock으로 `MockRepository<T>` 생성
- 아래 테스트 케이스 작성 (모두 실패 상태)
  - `Save_ValidEntity_StoresSuccessfully`
  - `Load_ExistingId_ReturnsEntity`
  - `Load_NonExistingId_ReturnsNull`
  - `Update_ExistingEntity_UpdatesSuccessfully`
  - `Delete_ExistingId_RemovesEntity`

---

### Phase 1-2 — [Green] JSON Repository 구현

> 테스트를 통과하는 최소한의 구현을 작성한다.

- `JsonRepository<T>` 구현 — JSON 파일 기반 CRUD
- 앱 재시작 후 데이터 유지 확인
- 모든 Phase 1-1 테스트 통과

---

### Phase 1-3 — [Red] 메인 메뉴 라우팅 테스트 작성

> 메뉴 입력에 따른 분기 동작을 테스트로 먼저 정의한다.

- gmock으로 각 메뉴 Controller Mock 생성
- 아래 테스트 케이스 작성 (모두 실패 상태)
  - `MainController_Input1_InvokesSampleController`
  - `MainController_Input2_InvokesOrderController`
  - `MainController_Input0_ExitsLoop`
  - `MainController_InvalidInput_ShowsErrorMessage`

---

### Phase 1-4 — [Green] 메인 메뉴 및 콘솔 I/O 구현

> 테스트를 통과하는 메인 루프와 입출력 헬퍼를 구현한다.

- `MainController` — 메뉴 번호 입력 처리 및 라우팅
- `ConsoleView` — 공통 입력/출력 헬퍼
- `MainMenuView` — 시스템 현황 요약 화면 출력
- 모든 Phase 1-3 테스트 통과

---

### Phase 1-5 — [Refactor] 리팩토링

> 동작은 유지하면서 코드 품질을 개선한다.

- `IRepository<T>` 인터페이스 추상화 검토
- 콘솔 I/O 유틸 중복 제거
- 매직 문자열 상수화
- 전체 Phase 1 테스트 재실행하여 통과 확인
