# ▶Project name : Defender◀
본 프로젝트는 Unreal Engine 5를 활용해 제작한 Third Person Shooting 방식의 디펜스 게임입니다.

## ✅Intro
- 본 프로젝트는 **코드의 재사용성**을 높히기 위해 **컴포넌트 기반으로 개발**했습니다.
- **유지보수**(수정과 확장)에 어려움이 없도록 **Class Wrapping**과 **디자인 패턴**을 적극 활용했습니다.
- 타인과 **원활한 협업**을 위해 **주석**, 블루프린트로의 적절한 **Serialize**, **CSV**를 통한 **데이터 관리**, **json**형식의 **데이터 입출력**을 활용했습니다.
- 메모리 최적화를 위해 **Object Pooling**을 적용했습니다.
- **라이브 서비스 중인 게임**들을 **모티브**해 여러가지 기능들을 구현했습니다.
- 버전관리는 Tortoise SVN을 사용했습니다.
## ✅개발기간
**2023/9/1(금) ~ 2024/1/10(수)**
## ✅개발 시 발생했던 이슈사항
### 1. LockOn 기능 시야 판별
임의의 위치에서 임의의 방향을 보고 있는 플레이어에서 MultiCapsuleTrace를 사용 시

Hit 되는 Actor들의 순서가 랜덤해서 사용자가 보고 있는 몬스터를 판별할 수 없었습니다.
### 2. UObject로 스킬 구현 시 Spawn, Tick 등 일부 World와 관련된 함수 구현 불가
최적화를 위해 UObject로 Skill을 구현했습니다.

하지만 이 경우 Wolrd와 관련된 로직을 작성할 수 없습니다.(ex. 소환, 지속효과 등)
### 3. 소모품 효과 적용
소모품은 인벤토리에서 TMap<FString, int32>로 관리했습니다.

->각 객체마다 포인터로 들고 있으면 메모리가 낭비되기에 CSV와 FString(CSV의 Row Name)과 int32(소모품의 개수)로 관리

사용 효과를 위해 멤버 함수를 static으로 구현 시 다형성을 사용할 수 없기 때문에 사용 효과를 적용하는데 어려움이 있었습니다.
## ✅이슈사항 해결 방법
### 1. 좌표계 변환
참조 : **Source\Defender\Private\Component\Player\DC_Targeting.cpp (UDC_Targeting::LockOn)**

Hit된 액터들의 위치정보를 기반으로 좌표계를 변환하여 시점에서 가장 가까운 적을 LockOn 하도록, 

나아가 좌우 측으로 Target을 변경하는 기능을 추가했습니다.
![좌표계변환](https://github.com/DongHyeon9/TPSPortfolio_Defender/blob/master/ForREADME/TransformCoord.jpg)
### 2. FTickableGameObject 다중상속
참조 : **Source\Defender\Public\Skills\DO_SkillBase.h**

FTickableGameObject을 상속받은 후 일부 함수를 구현하면 WorldTick에 등록되어 Actor보다 가벼우며, 

World와 관련된 함수를 사용할 수 있습니다.

자세한 내용은 기술 블로그와 깃허브에 기재

[FTickableGameObject 포스트](https://nicenewsdb.tistory.com/99)

[FTickableGameObject Github](https://github.com/DongHyeon9/Unreal/tree/World_UObjectTick)
### 3. 파생 클래스로 개별로 직 구현, CDO로 사용 로직 호출
참조 : **Source\Defender\Private\Component\Player\DC_Inventory.cpp (UDC_Inventory::RegistItem)**

언리얼에는 UObject마다 기본이 되는 CDO(Class Default Object)라는 것이 존재하기에 이를 활용,

각 아이템의 파생 클래스에서 효과로 직을 구현하고 사용 시 CDO 객체를 통해 사용 효과를 적용했습니다.

이로써 메모리 절약, 인 게임 내부의 아이템 관리, CSV를 통한 외부에서 관리 세 가지를 한 번에 잡을 수 있었습니다.
## ✅프로젝트 종료 후 알게 된 점
### 1. 오브젝트 풀링 구현
TQueue는 LockFree를 고려한 Node 기반 자료구조이기에 메모리 단편화로부터 자유롭지 못합니다.
### 1. 해결
오브젝트 풀링 구현 시 TArray를 사용해야 됩니다.
### 2. FTimerManager::SetTimer 사용 시 Packaged 파일에서 Fatal Error 발생
SetTimer 함수를 사용하면 원하는 시간 뒤에 이벤트를 호출할 수 있는 장점이 있습니다.

하지만 FTimerDelegate::CreateLambda로 이벤트를 설정 시 Editor에선 EngineTick이 이를 관리해 줬지만 Packaged 파일에선 엔진 코드가 제거됩니다.

때문에 객체가 제거되어 댕글링 포인터 이슈로 인해 Fatal Error가 발생하는 문제가 있었습니다.

[관련 공식 문서](https://dev.epicgames.com/documentation/ko-kr/unreal-engine/gameplay-timers-in-unreal-engine)
### 2-1 해결 : Delay Event를 Tick으로 구현(비추천)
타이머를 Tick에서 카운팅, 일정 시간 뒤에 호출할 수 있게 로직을 구현하면 해결할 수 있습니다.

단, 이 경우 코드의 복잡성이 증가할 수 있기에 추천하지 않습니다.
### 2-2 해결 : 멤버 함수 포인터 바인딩(추천)
FTimerHandle은 언리얼에서 자동으로 관리해 주는 객체이기 때문에 이를 활용해 멤버 함수를 선언,

객체 포인터와 멤버 함수 포인터를 인자로 넘겨주면 객체가 사라진 후에 호출 시도 시 호출되지 않습니다.
## ✅구현 내용
### 기능
1. json을 활용한 데이터 송수신(레벨이동, Save, Load)
2. FStreamableManager를 활용한 비동기 로드
3. Dynamic Keyboard Mapping(플레이어가 원하는 키보드에 스킬 배정)
### 플레이어
1. TPS 방식 이동
2. ControlRotation에 따른 플레이어 회전(배틀그라운드 기본 캐릭터 회전)
3. ControlRotation을 무시한 카메라 회전(배틀그라운드 자유 시야 모드)
4. LockOn 기능(좌표계 변환과 내적을 통한 플레이어 시점 가운데 적을 Targeting 하도록 구현)
5. LockOn 이동(플레이어 기준 현재 타겟에서 제일 가까운 좌 혹은 우측 적으로 타겟 변경 기능)
6. 앉기, 파쿠르, 구르기
7. State Machine을 통한 상태관리(피격, 스턴, 사망, 캐스팅 등)
8. 버프 기능 구현(곱연산, 합연산)
9. 캐릭터 바리에이션(애니메이션 리타기팅)
### UI
1. 플레이어 Infomation(HP, EXP, Level, 이름, 탄창갯수 등)
2. 상점 기능(골드, 판매, 구매 등)
3. 미니맵(RenderTargetTexture 활용)
### 인터렉션
1. 상점주인과 인터렉션
2. 폭팔물
3. 아이템(드랍)
### 아이템
1. 고정회복 포션
2. %회복 포션
3. 버프 포션(데미지 상승, 이동속도 증가)
4. 유틸 포션(경험치 획득)
### 무기
1. Zoom In, Zoom Out 기능(스나이퍼)
2. 격발시 랜덤성, 반동 기능 구현
3. 견착시 랜덤성 감소 기능
4. Projectile 방식, Tracing 방식 두 종류 무기 구현
5. 단발, 점사, 연사 기능 구현
6. 샷건 기능 구현(근접 타격시 데미지 증가)
### 스킬
상속을 통한 스킬 계층구조 관리(일반, 지속시간, 범위기반, 패시브 등)
1. Active : 플레이어를 따라다니는 불을 소환해 전방에 몬스터가 있을 경우 불화살을 날림
2. Active : 힐 존 생성
3. Active : 채널링 시간동안 전방으로 에너지파 방출(다단히트)
4. Active : 캐스팅 후 운석낙하
5. Active : 주변 범위 데미지 및 상태이상(기절)
6. Active : 버프(이동속도, 공격력증가)
7. Active : 주변 몬스터들이 느려짐(잔상 이펙트)
8. Active : 순간이동(오버워치 리퍼)
9. Passive : HP Regen
10. Passive : 이동속도 증가
11. Passive : 데미지 증가
### 몬스터
1. AI Perception과 Behaviour Tree를 활용
2. 피격 시 피격당한 방향을 쳐다봄
3. 소리가 들리면 그 곳으로 이동
4. 시야에 플레이어가 들어오면 추격
5. Strategy패턴을 활용한 공격기능
#### 일반
근접, 원거리(투사체), 원거리(캐스팅) 세 종류 몬스터 구현
1. 근접 : 연속공격을 하며 공격 도중 플레이어가 범위를 벗어나는 경우 공격을 취소하고 다시 플레이어를 추격
2. 원거리(투사체) : 공격 범위에 들어오면 투사체를 날려 공격함
3. 원거리(캐스팅) : 캐스팅시 플레이어 발 밑에 마법진이 소환되며 캐스팅이 끝날 시 해당 지역에 데미지를 줌
#### 보스
  - 1Page와 2Page로 나뉨
  - HP가 50% 미만이 될 시 2Page 시작
  - 각 페이지마다 시작 애니메이션 재생(시네마틱이 있을시 시네마틱 재생)
  - 피격상태나 스턴에 대해 면역
  - 여러개의 스킬이 각각 쿨타임 적용
  - 쿨타임이 종료된 스킬 중 랜덤으로 사용
##### 중간 보스
  - 1p 패턴 1 : 여러개의 투사체를 보스주변 랜덤한 위치에 소환해 플레이어에게 날림
  - 1p 패턴 2 : 대상으로 지정한 플레이어 주변에 여러개의 번개를 떨어뜨림
  - 1p 패턴 3 : 근접에서 3번의 공격(공격도중 플레이어가 멀어지면 다시 플레이어 추격)
  - 2페이지가 되면 2번패턴(번개)이 사라지며 추가로 다른 패턴들이 생김
  - 2p 패턴 1 : 보스 주변에 마법진이 생기며 들어오면 이동속도 감소
  - 2p 패턴 2 : 화염영역을 소환, 주변에 데미지를 줌
  - 2p 패턴 3 : 보스가 플레이어에게 돌진하고 무기를 휘둘러 맞은 플레이어는 스턴
  - 2p 패턴 4 : 무기를 휘둘러 주변 넓은범위에 데미지
##### 최종 보스
  - 1p 패턴 1 : 플레이어 발 밑에 마법진을 소환하고 폭발시켜 데미지를 줌
  - 1p 패턴 2 : 플레이어 주변에 여러개의 얼음기둥을 소환
  - 1p 패턴 3 : 등에서 여러개의 투사체를 하늘로 쏩니다. 이후 플레이어에게 유도되는 투사체를 날립니다.
  - 1p 패턴 4 : 랜덤한 위치에 일반 몬스터를 랜덤하게 소환
  - 2페이지가 되면 페이지 패턴이 그대로 유지되고 추가로 패턴이 생김
  - 2p 패턴 1 : 큰 얼음을 떨어뜨려 넓은 범위에 데미지
  - 2p 패턴 2 : 전방으로 에너지를 방출해 큰 데미지를 줌

---
플레이 영상은 유튜브에 있습니다.

[TPS 포트폴리오 영상](https://youtu.be/Npg4jeqO2nA)

-eod-
