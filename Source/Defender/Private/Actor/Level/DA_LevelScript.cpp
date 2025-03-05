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
	//�÷��̾� ��Ʈ�ѷ��� �������� ���� ������ �����ְ� ���� ������ �����Ѵ�
	auto player = GetWorld()->GetFirstPlayerController<ADAPC_InGame>();
	StartMonsterSpawn();
	if (player != nullptr) {
		player->ShowStageStartWidget();
		player->GetMainWidget()->SetLevelTimer(stageTimer);
	}

	if (stageTimer <= 0.f) return;

	//���������� ���� �������� �ð��� ǥ���Ѵ�
	//�ð��� �ٵǸ� �������� ���Ḧ ȣ���Ѵ�
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
	//Ÿ�̸� �ڵ��� �����Ѵ�
	if (stageTimeHandle.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(stageTimeHandle);
		stageTimeHandle.Invalidate();
	}

	//�������� ���������� �����ְ� �÷��̾ �������� �����
	auto playerController = GetWorld()->GetFirstPlayerController<ADAPC_InGame>();
	if (playerController != nullptr) {
		playerController->ShowStageClearWidget();
		auto playercharacter = Cast<ADA_Character>(playerController->GetPawn());
		if (playercharacter != nullptr)
			playercharacter->GetStatComponent()->SetState(ECHARACTER_STATE::INVINCIBILITY);
	}

	//���� ������ �����ϰ� ��� ���͸� �����Ѵ�
	EndMonsterSpawn();
	MonsterAllDestroy();

	//���� �ð��Ŀ� ���������� �����Ѵ�
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

	//���͸� ������ ����ŭ ��ȯ�Ѵ�
	if (monsterClass->ImplementsInterface(UDI_ObjectPoolingActor::StaticClass()))
		for (int32 i = 0; i < maxMonsterCount; ++i)
			SpawnMonster();

	//��ǥ�� �ִٸ� ��ǥ�� �״°Ϳ� ��������Ʈ�� �����Ѵ�
	if (missionGoal != nullptr)
		missionGoal->onAssetLoadCompleted.AddDynamic(this, &ADA_LevelScript::GoalAssetLoadCompleted);
}

void ADA_LevelScript::StartMonsterSpawn()
{
	//�����ð��̸��� ���͸� ��ȯ�Ѵ�
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
	//Ȯ���� �Ѵٸ� ���͸� �߰��� ��ȯ�ϰ�
	//Ȯ������ �ʴ´ٸ� false�� ��ȯ�Ѵ�
	if (monsterPool.IsEmpty())
		if (_CanExpand) ExpandPool();
		else return false;
	
	//��ȯ�� ������ ��ġ�� �����Ѵ�
	float xLoc = FMath::RandRange(spawnRange.X * -0.5f, spawnRange.X * 0.5f) + relativeSpawnLocation.X;
	float yLoc = FMath::RandRange(spawnRange.Y * -0.5f, spawnRange.Y * 0.5f) + relativeSpawnLocation.Y;
	FVector startLoction = FVector(xLoc, yLoc, ceilingHeight);
	FVector endLocation = FVector(xLoc, yLoc, relativeSpawnLocation.Z);

	FHitResult hitResult{};
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{};
	TEnumAsByte<EObjectTypeQuery> worldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
	ObjectTypes.Add(worldStatic);

	//��ȿ�� ��ġ�� ã�������� Ʈ���̽��� �Ѵ�
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

	//��ȿ�� ��ġ�� ã���� �� ��ġ�� ���͸� ��ȯ�Ѵ�
	//��ȯ�ϴ� ���͸� ������ Ÿ�԰� ������ ������ ��ȯ�ȴ�
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
	//��� ���͸� �����Ѵ�
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
	//������Ʈ Ǯ�� ����ִٸ� Ȯ���Ѵ�
	if (monsterPool.IsEmpty()) ExpandPool();

	auto monster = *monsterPool.Peek();
	monsterPool.Pop();
	return monster;
}

void ADA_LevelScript::AdmissionPlayer(ADAPC_InGame* _NewPlayer)
{
	//���� �����ϴ� �÷��̾ ������ �����Ѵ�
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
	//���������� �����Ѵ�
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
	//�÷��̾ ������ ������ ������ �����Ѵ�
	if (_NewState == ECHARACTER_STATE::DEAD) {
		auto playerCharacter = Cast<ADA_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if (playerCharacter == nullptr) return;

		playerCharacter->GetStatComponent()->onChangeState.RemoveDynamic(this, &ADA_LevelScript::PlayerStateChange);
		EndMonsterSpawn();
	}
}

void ADA_LevelScript::GoalAssetLoadCompleted()
{
	//��ǥ ���Ͱ� ������ ���������� �����Ѵ�
	auto animInstace = Cast<UDO_AnimInstance>(missionGoal->GetMesh()->GetAnimInstance());
	animInstace->onDeadEnd.AddDynamic(this, &ADA_LevelScript::StageEnd);
}

void ADA_LevelScript::ExpandPool()
{
	//������Ʈ Ǯ�� Ȯ���Ѵ�
	int32 currentSize = maxMonsterCount++;
	maxMonsterCount *= 1.5;
	for (int32 i = currentSize; i < maxMonsterCount; ++i)
		SpawnMonster();
}

void ADA_LevelScript::SpawnMonster()
{
	//���͸� ��ȯ�ϰ� ť�� �ִ´�
	float xLoc = FMath::RandRange(spawnRange.X * -0.5, spawnRange.X * 0.5) + relativeSpawnLocation.X;
	float yLoc = FMath::RandRange(spawnRange.Y * -0.5, spawnRange.Y * 0.5) + relativeSpawnLocation.Y;

	FVector spawnLoc = FVector(xLoc, yLoc, relativeSpawnLocation.Z + ceilingHeight);

	auto monster = GetWorld()->SpawnActor<ADA_Monster>(monsterClass, spawnLoc, FRotator::ZeroRotator);
	DFCHECK(monster != nullptr);
	IDI_ObjectPoolingActor::Execute_ActiveSetting(monster, false);
	monster->onDeActivate.AddDynamic(this, &ADA_LevelScript::MonsterDeActive);
	monsterPool.Enqueue(monster);
}
