# PRD - 반도체 시료 생산주문관리 시스템

## 1. 배경 및 목적

반도체 회사 S-Semi는 다양한 시료(Sample)를 생산하여 연구소, 팹리스 업체, 대학 연구실 등에 납품한다.
주문량 급증으로 엑셀·메모장 기반의 수작업 관리가 한계에 달해, 주문 상태와 재고·공정 현황을 체계적으로
추적할 수 있는 **콘솔 기반 생산주문관리 시스템**을 개발한다.

---

## 2. 이해관계자 및 역할

| 역할 | 책임 |
|------|------|
| 고객 | 필요한 시료를 이메일로 요청 |
| 주문 담당자 | 요청에 맞게 주문서 작성 및 관리 |
| 생산 담당자 | 시료 등록, 주문 수신 후 승인 또는 거절 처리 |

---

## 3. 주문 상태 모델

모든 주문은 아래 5가지 상태 중 하나를 가진다.

| 상태 | 의미 |
|------|------|
| RESERVED | 주문 접수 |
| REJECTED | 주문 거절 (모니터링 제외) |
| PRODUCING | 승인 완료 + 재고 부족으로 생산 중 |
| CONFIRMED | 승인 완료 + 출고 대기 중 |
| RELEASED | 출고 완료 |

**상태 전이 흐름**

```
RESERVED → (승인) → 재고 충분: CONFIRMED → RELEASED
                 → 재고 부족: PRODUCING → CONFIRMED → RELEASED
         → (거절) → REJECTED
```

---

## 4. 핵심 기능 요구사항

### 4.1 시료 관리
- 시료 등록: 시료 ID, 이름, 평균 생산시간(min/ea), 수율 입력
- 시료 목록 조회: 등록된 시료 전체 + 현재 재고 수량 표시
- 시료 검색: 이름 등 속성으로 검색

> 수율 = 정상 시료 수 / 총 생산 시료 수

### 4.2 시료 주문 (접수)
- 입력값: 시료 ID, 고객명, 주문 수량
- 접수 즉시 상태 RESERVED로 등록

### 4.3 주문 승인/거절
- RESERVED 목록을 조회하고 각 주문을 개별 처리
- 승인 시 재고 상황에 따라 자동 분기
  - 재고 충분 → CONFIRMED
  - 재고 부족 → 생산라인 자동 등록 + PRODUCING
- 거절 시 즉시 REJECTED

### 4.4 생산라인
- 단일 라인, FIFO 스케줄링
- 실 생산량: `ceil(부족분 / (수율 × 0.9))`
- 총 생산 시간: `평균 생산시간 × 실 생산량`
- 생산 완료 시 PRODUCING → CONFIRMED 자동 전환
- 현재 생산 중인 주문 정보 및 대기 큐 목록 표시

### 4.5 모니터링
- 주문량: 상태별(RESERVED / CONFIRMED / PRODUCING / RELEASED) 건수 표시
- 재고량: 시료별 재고 수량 + 상태 표기
  - 여유(재고 충분) / 부족(재고 부족) / 고갈(재고 0)

### 4.6 출고 처리
- CONFIRMED 상태 주문 목록 조회 후 선택
- 출고 실행 시 CONFIRMED → RELEASED 전환

---

## 5. 비기능 요구사항

| 항목 | 내용 |
|------|------|
| 인터페이스 | 콘솔(CLI) 기반 |
| 데이터 영속성 | 앱 재시작 후에도 데이터 유지 (JSON) |
| 아키텍처 | MVC 패턴 (Model / Controller / View 패키지 분리) |
| 빌드 | MS Build 옵션 변경 금지 |

---

## 6. 메인 메뉴 구성

| 번호 | 메뉴명 |
|------|--------|
| 1 | 시료 관리 |
| 2 | 시료 주문 |
| 3 | 주문 승인/거절 |
| 4 | 모니터링 |
| 5 | 생산라인 조회 |
| 6 | 출고 처리 |
| 0 | 종료 |

메인 메뉴 진입 시 전체 요약 정보(등록 시료 수, 총 재고, 전체 주문 수, 생산라인 대기 수) 표시.

---

## 7. 참조 PoC 항목

본 프로젝트 개발 전 기능을 검증하기 위해 아래 PoC를 별도 Repository로 선행 개발한다.

| PoC 항목 | Repository | 설명 |
|----------|-----------|------|
| MVC 스켈레톤 코드 | [ConsoleMVC-wally3017](https://github.com/wally3017-ctrl/ConsoleMVC-wally3017.git) | Model / Controller / View 패키지 구조와 역할 분리 검증 |
| 데이터 영속성 처리 | [DataPersistence-wally3017](https://github.com/wally3017-ctrl/DataPersistence-wally3017.git) | 파일/JSON 기반 데이터 저장·불러오기 및 CRUD 검증 |
| 데이터 모니터링 Tool | [DataMonitor-wally3017](https://github.com/wally3017-ctrl/DataMonitor-wally3017.git) | 저장된 데이터 상태를 콘솔에서 실시간 조회하는 관리자 도구 |
| Dummy 데이터 생성 Tool | [DummyDataGenerator-wally3017](https://github.com/wally3017-ctrl/DummyDataGenerator-wally3017.git) | 테스트용 Dummy Data 생성 및 DB 추가 도구 |

---

## 8. 관련 문서

- [PLAN.md](../temp_docs/PLAN.md) — 개발 계획 및 TDD Phase 일정