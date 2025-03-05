#include "Actor/Level/DA_LevelScript.h"
#include "Actor/Monster/DA_Monster.h"
#include "Interface/DI_ObjectPoolingActor.h"
#include "Actor/Player/Controller/DAPC_InGame.h"
#include "Actor/Player/DA_Player.h"
#include "Component/Share/DC_Stat.h"
#include "Widget/InGame/Shop/DWP_Shop.h"
#include "Widget/InGame/Information/DWP_MainWidget.h"
#include "Actor/DO_AnimInstance.h"
#include "Server/DA_GameInstance.h"

#include "Kismet/GameplayStatics.h"

ADA_LevelScript::ADA_LevelScript()
{
	shopTimer = 30.f;
	stageTimer = 300.f;
	levelOpenTimer = 5.f;
	monsterSpawnType.Add(EMONSTER_TYPE::MELEE);
	bIsAutoStart = true;
}

void ADA_LevelScript::StageStart()
{
	//플레이어 컨트롤러에 스테이지 시작 위젯을 보여주고 몬스터 스폰을 시작한다
	auto player = GetWorld()->GetFirstPlayerController<ADAPC_InGame>();
	StartMonsterSpawn();
	if (player != nullptr) {
		player->ShowStageStartWidget();
		player->GetMainWidget()->SetLevelTimer(stageTimer);
	}

	if (stageTimer <= 0.f) return;

	//메인위젯에 현재 스테이지 시간을 표시한다
	//시간이 다되면 스테이지 종료를 호출한다
	currentTime = stageTimer;

	auto gameInstance = GetGameInstance<UDA_GameInstance>();
	DFCHECK(gameInstance != nullptr);
	gameInstance->RegistTimerHandle(&stageTimeHandle);

	GetWorld()->GetTimerManager().SetTimer(stageTimeHandle, FTimerDelegate::CreateLambda([&]()
		{
			auto player = GetWorld()->GetFirstPlayerController<ADAPC_InGame>();
			player->GetMainWidget()->SetLevelTimer(--currentTime);

			if (currentTime < KINDA_SMALL_NUMBER &&
				stageTimeHandle.IsValid()) {
				GetWorld()->GetTimerManager().ClearTimer(stageTimeHandle);
				stageTimeHandle.Invalidate();
				StageEnd();
			}
		}), 1.f, true);
}

void ADA_LevelScript::StageEnd()
{
	//타이머 핸들을 제거한다
	if (stageTimeHandle.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(stageTimeHandle);
		stageTimeHandle.Invalidate();
	}

	//스테이지 종료위젯을 보여주고 플레이어를 무적으로 만든다
	auto playerController = GetWorld()->GetFirstPlayerController<ADAPC_InGame>();
	if (playerController != nullptr) {
		playerController->ShowStageClearWidget();
		auto playercharacter = Cast<ADA_Character>(playerController->GetPawn());
		if (playercharacter != nullptr)
			playercharacter->GetStatComponent()->SetState(ECHARACTER_STATE::INVINCIBILITY);
	}

	//몬스터 스폰을 종료하고 모든 몬스터를 삭제한다
	EndMonsterSpawn();
	MonsterAllDestroy();

	//일정 시간후에 다음레벨을 시작한다
	FTimerHandle levelOpenHandle{};
	GetWorld()->GetTimerManager().SetTimer(levelOpenHandle, FTimerDelegate::CreateLambda([&]()
		{
			auto player = GetWorld()->GetFirstPlayerController<ADAPC_InGame>();
			if (player != nullptr) {
				player->OpenLevel(nextLevelName);
			}
		}), levelOpenTimer, false);
}

void ADA_LevelScript::BeginPlay()
{
	Super::BeginPlay();

	DFCHECK(!monsterSpawnType.IsEmpty());

	//몬스터를 지정한 수만큼 소환한다
	if (monsterClass->ImplementsInterface(UDI_ObjectPoolingActor::StaticClass()))
		for (int32 i = 0; i < maxMonsterCount; ++i)
			SpawnMonster();

	//목표가 있다면 목표가 죽는것에 델리게이트를 연결한다
	if (missionGoal != nullptr)
		missionGoal->onAssetLoadCompleted.AddDynamic(this, &ADA_LevelScript::GoalAssetLoadCompleted);
}

void ADA_LevelScript::StartMonsterSpawn()
{
	//스폰시간이마다 몬스터를 소환한다
	if (spawnDelay > 0.f) {

		auto gameInstance = GetGameInstance<UDA_GameInstance>();
		DFCHECK(gameInstance != nullptr);
		gameInstance->RegistTimerHandle(&monsterSpawnHandle);

		GetWorld()->GetTimerManager().SetTimer(monsterSpawnHandle, FTimerDelegate::CreateLambda([&]() {

			SpawnMonsterRandomLocation();

			}), spawnDelay, true);
	}
}

void ADA_LevelScript::EndMonsterSpawn()
{
	if (!monsterSpawnHandle.IsValid()) return;
	GetWorld()->GetTimerManager().ClearTimer(monsterSpawnHandle);
	monsterSpawnHandle.Invalidate();
}

bool ADA_LevelScript::SpawnMonsterRandomLocation(bool _CanExpand)
{
	//확장을 한다면 몬스터를 추가로 소환하고
	//확장하지 않는다면 false를 반환한다
	if (monsterPool.IsEmpty())
		if (_CanExpand) ExpandPool();
		else return false;
	
	//소환될 랜덤한 위치를 결정한다
	float xLoc = FMath::RandRange(spawnRange.X * -0.5f, spawnRange.X * 0.5f) + relativeSpawnLocation.X;
	float yLoc = FMath::RandRange(spawnRange.Y * -0.5f, spawnRange.Y * 0.5f) + relativeSpawnLocation.Y;
	FVector startLoction = FVector(xLoc, yLoc, ceilingHeight);
	FVector endLocation = FVector(xLoc, yLoc, relativeSpawnLocation.Z);

	FHitResult hitResult{};
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{};
	TEnumAsByte<EObjectTypeQuery> worldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
	ObjectTypes.Add(worldStatic);

	//유효한 위치를 찾을때까지 트레이싱을 한다
	while (!UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		startLoction,
		endLocation,
		ObjectTypes,
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::ForDuration,
		hitResult,
		true));

	//유효한 위치를 찾으면 그 위치에 몬스터를 소환한다
	//소환하는 몬스터를 랜덤한 타입과 랜덤한 레벨로 소환된다
	auto monster = *monsterPool.Peek();
	monsterPool.Pop();

	FVector spawnLocation = hitResult.Location;
	spawnLocation.Z += (monster->GetDefaultHalfHeight() * monster->GetActorScale3D().Z);
	FRotator spawnRotation = FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f);

	int32 monsterType = FMath::RandRange(0, monsterSpawnType.Num() - 1);
	int32 monsterLevel = FMath::RandRange(minMonsterLevel, maxMonsterLevel);

	monster->TeleportTo(spawnLocation, spawnRotation);
	monster->InitializeMonster(monsterSpawnType[monsterType], monsterLevel);
	IDI_ObjectPoolingActor::Execute_ActiveSetting(monster, true);
	return true;
}

void ADA_LevelScript::MonsterAllDestroy()
{
	//모든 몬스터를 삭제한다
	TArray<AActor*> outMonsters{};
	UGameplayStatics::GetAllActorsOfClass(this, ADA_Monster::StaticClass(), outMonsters);

	for (auto outMonster : outMonsters)
		if (outMonster != nullptr) {
			auto monster = Cast<ADA_Monster>(outMonster);
			if (monster != nullptr)
				monster->onDeActivate.RemoveDynamic(this, &ADA_LevelScript::MonsterDeActive);
			outMonster->Destroy();
		}
	monsterPool.Empty();
}

ADA_Monster* ADA_LevelScript::GetMonster()
{
	//오브젝트 풀이 비어있다면 확장한다
	if (monsterPool.IsEmpty()) ExpandPool();

	auto monster = *monsterPool.Peek();
	monsterPool.Pop();
	return monster;
}

void ADA_LevelScript::AdmissionPlayer(ADAPC_InGame* _NewPlayer)
{
	//새로 입장하는 플레이어도 게임을 진행한다
	if (_NewPlayer != nullptr) {
		auto playerCharacter = Cast<ADA_Player>(_NewPlayer->GetPawn());
		auto shopWidget = _NewPlayer->ShowShopWidget(shopTimer);
		shopWidget->onClose.AddDynamic(this, &ADA_LevelScript::LevelStart);
		playerCharacter->GetStatComponent()->onChangeState.AddDynamic(this, &ADA_LevelScript::PlayerStateChange);
	}
}

void ADA_LevelScript::MonsterDeActive(ADA_Monster* _Monster)
{
	monsterPool.Enqueue(_Monster);
}

void ADA_LevelScript::LevelStart()
{
	//스테이지를 시작한다
	auto player = GetWorld()->GetFirstPlayerController<ADAPC_InGame>();
	if (player != nullptr)
		player->GetShopWidget()->onClose.RemoveDynamic(this, &ADA_LevelScript::LevelStart);

	FTimerHandle stageStartHandle{};
	GetWorld()->GetTimerManager().SetTimer(stageStartHandle, FTimerDelegate::CreateLambda([&]()
		{
			StageStart();
		}), 0.5f, false);
}

void ADA_LevelScript::PlayerStateChange(ECHARACTER_STATE _NewState)
{
	//플레이어가 죽으면 몬스터의 스폰을 종료한다
	if (_NewState == ECHARACTER_STATE::DEAD) {
		auto playerCharacter = Cast<ADA_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if (playerCharacter == nullptr) return;

		playerCharacter->GetStatComponent()->onChangeState.RemoveDynamic(this, &ADA_LevelScript::PlayerStateChange);
		EndMonsterSpawn();
	}
}

void ADA_LevelScript::GoalAssetLoadCompleted()
{
	//목표 몬스터가 죽으면 스테이지를 종료한다
	auto animInstace = Cast<UDO_AnimInstance>(missionGoal->GetMesh()->GetAnimInstance());
	animInstace->onDeadEnd.AddDynamic(this, &ADA_LevelScript::StageEnd);
}

void ADA_LevelScript::ExpandPool()
{
	//오브젝트 풀을 확장한다
	int32 currentSize = maxMonsterCount++;
	maxMonsterCount *= 1.5;
	for (int32 i = currentSize; i < maxMonsterCount; ++i)
		SpawnMonster();
}

void ADA_LevelScript::SpawnMonster()
{
	//몬스터를 소환하고 큐에 넣는다
	float xLoc = FMath::RandRange(spawnRange.X * -0.5, spawnRange.X * 0.5) + relativeSpawnLocation.X;
	float yLoc = FMath::RandRange(spawnRange.Y * -0.5, spawnRange.Y * 0.5) + relativeSpawnLocation.Y;

	FVector spawnLoc = FVector(xLoc, yLoc, relativeSpawnLocation.Z + ceilingHeight);

	auto monster = GetWorld()->SpawnActor<ADA_Monster>(monsterClass, spawnLoc, FRotator::ZeroRotator);
	DFCHECK(monster != nullptr);
	IDI_ObjectPoolingActor::Execute_ActiveSetting(monster, false);
	monster->onDeActivate.AddDynamic(this, &ADA_LevelScript::MonsterDeActive);
	monsterPool.Enqueue(monster);
}
