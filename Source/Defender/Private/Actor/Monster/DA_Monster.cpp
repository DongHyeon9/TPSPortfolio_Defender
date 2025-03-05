#include "Actor/Monster/DA_Monster.h"
#include "Component/Share/DC_AnimMontage.h"
#include "Widget/InGame/Monster/DW_MonsterHP.h"
#include "Component/Share/DC_Stat.h"
#include "Actor/Monster/AI/DA_AIController.h"
#include "Actor/Monster/DOA_Monster.h"
#include "Component/Share/DC_Skill.h"
#include "Component/Share/DC_ProjectileManager.h"
#include "Server/DA_GameInstance.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/AssetManager.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/WidgetComponent.h"
#include "LevelSequenceActor.h"
#include "MovieSceneSequencePlayer.h"
#include "Kismet/GameplayStatics.h"

ADA_Monster::ADA_Monster()
{
	lockOnArea = CreateDefaultSubobject<USceneComponent>(TEXT("LockOnArea"));
	lockOnArea->SetupAttachment(GetMesh());

	weaponComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponComp"));
	weaponComp->SetupAttachment(GetMesh());

	hpWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar"));
	hpWidgetComp->SetupAttachment(RootComponent);
	hpWidgetComp->SetHiddenInSceneCapture(true);
	hpWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	hpWidgetComp->SetVisibility(false);

	Tags.Add(TEXT("Monster"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ADA_AIController::StaticClass();

	bIsActive = false;
	beginCinematicIndex = -1;
}

void ADA_Monster::InitializeMonster(EMONSTER_TYPE _MonsterType, int32 _NewLevel, float _DeactivateDelay)
{
	//������ Ÿ���� �����ϰ� �� Ÿ�Կ� ���� ���������̺��� �ҷ��´�
	monsterType = _MonsterType;
	deactivateDelay = _DeactivateDelay;

	const UEnum* monsterTypeEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EMONSTER_TYPE"), true);
	DFCHECK(monsterTypeEnum != nullptr);

	FString monsterTypeString = monsterTypeEnum->GetDisplayNameTextByIndex((int32)monsterType).ToString();
	monsterData = monsterDataTable->FindRow<FDFMonsterData>(*monsterTypeString, TEXT(""));

	DFCHECK(monsterData != nullptr);

	GetAnimMontageComponent()->SetAnimMontage(monsterData->animMontageMap);
	GetStatComponent()->SetLevel(_NewLevel);

	//�ҷ��� �����͸� ������� ������ AI��Ʈ�ѷ��� �������ش�
	monsterAIController->InitializeAIController(monsterData->monsterActiveSkills);
}

void ADA_Monster::MeshActive_Implementation(bool _bIsActive)
{
	DFLOG_N(Display);
	GetMesh()->SetHiddenInGame(!_bIsActive, true);
	if(_bIsActive)
		lockOnArea->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("LockOnLocation"));

	else
		lockOnArea->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));	
}

void ADA_Monster::CollisionActive_Implementation(bool _bIsActive)
{
	DFLOG_N(Display);
	if (_bIsActive) {
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else {
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	GetCapsuleComponent()->SetHiddenInGame(!_bIsActive);

#endif
}

void ADA_Monster::ActiveSetting_Implementation(bool _bIsActive)
{
	DFLOG_N(Display);
	bIsActive = _bIsActive;

	if (bIsActive) {
		//Ȱ��ȭ�� ������ ���̺� ����ִ� ������ ���� ������ ������ �����Ѵ�
		IDI_ObjectPoolingActor::Execute_CollisionActive(this, true);
		auto skillComponent = GetSkillComponent();
		
		hpWidgetComp->SetRelativeLocation(monsterData->widgetLocation);

		GetMesh()->SetRelativeScale3D(FVector(monsterData->scale));

		auto monsterActives = monsterData->monsterActiveSkills;
		auto monsterPassives = monsterData->monsterPassiveSkills;

		for (auto active : monsterActives)
			skillComponent->AddActiveSkillbyClass(active.monsterSkillKey, active.activeSkillClass);

		for (auto passive : monsterPassives)
			skillComponent->AddPassiveSkillbyClass(passive.monsterSkillKey, passive.passiveSkillClass);
		
		//������ �񵿱�� �ε��Ѵ�
		monsterAssetsToLoad.Add(monsterData->skeletalMesh.ToSoftObjectPath());
		monsterAssetsToLoad.Add(monsterData->animBP.ToSoftObjectPath());
		monsterAssetsToLoad.Add(monsterData->monsterHPWidgetClass.ToSoftObjectPath());
		monsterAssetsToLoad.Add(monsterData->monsterImage.ToSoftObjectPath());
		monsterAssetsToLoad.Add(monsterData->blackBoard.ToSoftObjectPath());
		monsterAssetsToLoad.Add(monsterData->behaviorTree.ToSoftObjectPath());
		monsterAssetsToLoad.Add(monsterData->weaponMesh.ToSoftObjectPath());
		assetStreamingHandle =
			UAssetManager::GetStreamableManager().RequestAsyncLoad(
				monsterAssetsToLoad,
				FStreamableDelegate::CreateUObject(this,
					&ADA_Monster::OnAssetLoadCompleted));
	}
	else {
		//��Ȱ��ȭ�� HP������ ���ְ�
		//�����ߴ� ��������Ʈ���� �������ش�
		if (hpWidgetComp->IsVisible()) {
			hpWidgetComp->SetVisibility(false);
			GetStatComponent()->onChangeHP.RemoveDynamic(this, &ADA_Monster::OnChangeHP);
		}
		if (monsterWidget != nullptr)
			monsterWidget->RemoveInViewport();
		auto monsterAnimInstance = Cast<UDO_AnimInstance>(GetMesh()->GetAnimInstance());
		if (monsterAnimInstance != nullptr) {
			monsterAnimInstance->onHitStart.RemoveDynamic(this, &ADA_Monster::HitStart);
			monsterAnimInstance->onHitEnd.RemoveDynamic(this, &ADA_Monster::HitEnd);
			monsterAnimInstance->onStunEnd.RemoveDynamic(this, &ADA_Monster::StunEnd);
			monsterAnimInstance->onDeadEnd.RemoveDynamic(this, &ADA_Monster::DeadEnd);
		}
		//��ų,�߻�ü���� ����ϱ� ������ ������ ���밡���� ���·� ����� �ð����� ��ҵ� ��Ȱ��ȭ �Ѵ�
		GetSkillComponent()->RemoveAllActive();
		GetSkillComponent()->RemoveAllPassive();
		GetProjectileManagerComponent()->ResetProjectile();
		GetAnimMontageComponent()->StopAnimMontage();
		onDeActivate.Broadcast(this);
		IDI_ObjectPoolingActor::Execute_MeshActive(this, false);
	}
}

void ADA_Monster::PlayCinematic(int32 _Index)
{
	DFCHECK(!cinematics.IsEmpty() &&
		_Index >= 0 &&
		cinematics.Num() > _Index);

	//��� �÷��̾ �������·� �����
	auto pIter = GetWorld()->GetPlayerControllerIterator();
	for (; pIter; ++pIter) {
		auto player = Cast<ADA_Character>(pIter->Get()->GetPawn());
		if (player != nullptr)
			player->GetStatComponent()->SetState(ECHARACTER_STATE::INVINCIBILITY);
	}

	//���� ������̴� �ִԸ�Ÿ�ֿ� AI�� �����ϰ� �ε����� �´� �ó׸�ƽ�� ����Ѵ�
	monsterAIController->StopAI(TEXT("Cinematic"));
	GetAnimMontageComponent()->StopAnimMontage();
	FString cinematicName = TEXT("Cinematic") + FString::FromInt(_Index);
	GetAnimMontageComponent()->PlayAnimMontage(cinematicName);
	cinematics[_Index]->GetSequencePlayer()->OnFinished.AddDynamic(this, &ADA_Monster::OnEndCinematic);
	cinematics[_Index]->GetSequencePlayer()->Play();
	
}

void ADA_Monster::OnEndCinematic()
{
	//�����ߴ� ��������Ʈ���� �������ش�
	for (auto cinematic : cinematics)
		cinematic->GetSequencePlayer()->OnFinished.RemoveDynamic(this, &ADA_Monster::OnEndCinematic);

	//��� �÷��̾��� �������¸� �����Ѵ�
	auto pIter = GetWorld()->GetPlayerControllerIterator();
	for (; pIter; ++pIter) {
		auto player = Cast<ADA_Character>(pIter->Get()->GetPawn());
		if (player != nullptr)
			player->GetStatComponent()->SetState(ECHARACTER_STATE::NORMAL);
	}

	//AI�� �ٽ� �����Ѵ�
	GetAnimMontageComponent()->StopAnimMontage();
	monsterAIController->RunAI();
}

void ADA_Monster::PossessedBy(AController* _NewController)
{
	Super::PossessedBy(_NewController);

	monsterAIController = Cast<ADA_AIController>(_NewController);
	DFCHECK(monsterAIController != nullptr);

	IDI_ObjectPoolingActor::Execute_CollisionActive(this, false);
	IDI_ObjectPoolingActor::Execute_MeshActive(this, false);
	GetStatComponent()->onChangeState.AddDynamic(this, &ADA_Monster::OnStateChange);
}

void ADA_Monster::BeginPlay()
{
	Super::BeginPlay();

	auto gameInstance = GetGameInstance<UDA_GameInstance>();
	DFCHECK(gameInstance != nullptr);
	gameInstance->RegistTimerHandle(&deactivateTimerHandle);

	if (bIsSpawned) {
		InitializeMonster(monsterType, characterStartLevel, deactivateDelay);
		IDI_ObjectPoolingActor::Execute_ActiveSetting(this, true);
	}
}

void ADA_Monster::OnAssetLoadCompleted()
{
	//�ε�� ������ �����´�
	DFLOG_N(Display);
	TArray<UObject*> loadedAssets{};
	assetStreamingHandle->GetLoadedAssets(loadedAssets);
	assetStreamingHandle.Reset();

	//�ε�� ���µ��� ������ �°� �������ش�
	for (auto loadedAsset : loadedAssets) {
		auto sk_mesh = Cast<USkeletalMesh>(loadedAsset);
		if (sk_mesh != nullptr) 
			GetMesh()->SetSkeletalMesh(sk_mesh);

		auto abp_AnimInst = Cast<UAnimBlueprintGeneratedClass>(loadedAsset);
		if (abp_AnimInst != nullptr) {
			GetMesh()->SetAnimInstanceClass(abp_AnimInst);
			auto monsterAnim = CastChecked<UDOA_Monster>(GetMesh()->GetAnimInstance());
			GetAnimMontageComponent()->SetOwnerAnimInstance(monsterAnim);
			monsterAnim->onHitStart.AddDynamic(this, &ADA_Monster::HitStart);
			monsterAnim->onHitEnd.AddDynamic(this, &ADA_Monster::HitEnd);
			monsterAnim->onStunEnd.AddDynamic(this, &ADA_Monster::StunEnd);
			monsterAnim->onDeadEnd.AddDynamic(this, &ADA_Monster::DeadEnd);
		}

		auto monsterWidgetAsset = Cast<UWidgetBlueprintGeneratedClass>(loadedAsset);
		if (monsterWidgetAsset != nullptr) {
			hpWidgetComp->SetWidgetClass(monsterWidgetAsset);
			monsterWidget = Cast<UDW_MonsterHP>(hpWidgetComp->GetWidget());
			DFCHECK(monsterWidget != nullptr);
			monsterWidget->SetOwnerCharacter(this);
			monsterWidget->InitializeWidget();
		}

		auto monsterImage = Cast<UTexture2D>(loadedAsset);
		if (monsterImage != nullptr) {
			DFCHECK(monsterWidget != nullptr);
			monsterWidget->SetMonsterImage(monsterImage);
		}

		auto bb_blackBoard = Cast<UBlackboardData>(loadedAsset);
		if (bb_blackBoard != nullptr) {
			DFCHECK(monsterAIController != nullptr);
			UBlackboardComponent* blackBoardComp = monsterAIController->GetBlackboardComponent();
			monsterAIController->UseBlackboard(bb_blackBoard, blackBoardComp);
		}

		auto bt_behavior = Cast<UBehaviorTree>(loadedAsset);
		if (bt_behavior != nullptr) {
			DFCHECK(monsterAIController != nullptr);
			monsterAIController->RunBehaviorTree(bt_behavior);
		}

		auto weapon = Cast<UStaticMesh>(loadedAsset);
		if (weapon != nullptr) {
			weaponComp->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
			weaponComp->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("WeaponSocket"));
			weaponComp->SetRelativeTransform(monsterData->weaponRelativeTransform);
			weaponComp->SetStaticMesh(weapon);
		}
	}

	//AI���� ������Ʈ�� ������ �������ش�
	auto aiPerception = monsterAIController->GetAIPerceptionComponent();
	UAISenseConfig_Sight* sight = Cast<UAISenseConfig_Sight>(aiPerception->GetSenseConfig(monsterAIController->GetSightID()));

	if (sight != nullptr) {
		sight->SightRadius = monsterData->sightRadius;
		sight->LoseSightRadius = monsterData->loseSightRadius;
		sight->PeripheralVisionAngleDegrees = monsterData->sightAngle;
		sight->SetMaxAge(monsterData->maxAgeSight);
	}

	UAISenseConfig_Hearing* hearing = Cast<UAISenseConfig_Hearing>(aiPerception->GetSenseConfig(monsterAIController->GetHearingID()));

	if (hearing != nullptr) {
		hearing->HearingRange = monsterData->hearingRange;
		hearing->SetMaxAge(monsterData->maxAgeHearing);
	}

	//���͸� Ȱ��ȭ �����ش�
	aiPerception->RequestStimuliListenerUpdate();

	GetStatComponent()->onChangeHP.RemoveDynamic(this, &ADA_Monster::OnChangeHP);
	GetStatComponent()->onChangeHP.AddDynamic(this, &ADA_Monster::OnChangeHP);

	GetStatComponent()->SetState(ECHARACTER_STATE::NORMAL);

	onAssetLoadCompleted.Broadcast();

	if (bIsSpawned)
		PlayCinematic(beginCinematicIndex);

	else
		monsterAIController->RunAI();

	IDI_ObjectPoolingActor::Execute_MeshActive(this, true);
}

void ADA_Monster::OnChangeHP(float _MaxHP, float _CurrentHP)
{
	//ü���� �������� ������ �����ش�
	if (_CurrentHP < _MaxHP &&
		0.f < _CurrentHP) {
		switch (monsterType)
		{
		case EMONSTER_TYPE::MELEE:
		case EMONSTER_TYPE::PROJECTILE:
		case EMONSTER_TYPE::CAST: {
			hpWidgetComp->SetVisibility(true);
			hpWidgetComp->RequestRedraw();
			break;
		}
		case EMONSTER_TYPE::MIDDLE_BOSS:
		case EMONSTER_TYPE::FINAL_BOSS: {
			monsterWidget->ShowInViewport(VO_BOSS_HP);
			break;
		}
		}
	}
}

void ADA_Monster::HitStart()
{
	monsterAIController->StopAI(TEXT("Hit"));
}

void ADA_Monster::HitEnd()
{
	monsterAIController->RunAI();
}

void ADA_Monster::StunEnd()
{
	monsterAIController->RunAI();
}

void ADA_Monster::DeadEnd()
{
	GetWorld()->GetTimerManager().SetTimer(deactivateTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			IDI_ObjectPoolingActor::Execute_ActiveSetting(this, false);
			if (deactivateTimerHandle.IsValid()) {
				GetWorld()->GetTimerManager().ClearTimer(deactivateTimerHandle);
				deactivateTimerHandle.Invalidate();
			}
		}), deactivateDelay, false);
}

void ADA_Monster::OnStateChange(ECHARACTER_STATE _NewState)
{
	switch (_NewState)
	{
	case ECHARACTER_STATE::STUN: {
		monsterAIController->StopAI(TEXT("Stun"));
		break;
	}
	case ECHARACTER_STATE::DEAD: {
		monsterAIController->StopAI(TEXT("Dead"));
		IDI_ObjectPoolingActor::Execute_CollisionActive(this, false);
		break;
	}
	}
}
