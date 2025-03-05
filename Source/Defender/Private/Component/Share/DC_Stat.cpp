#include "Component/Share/DC_Stat.h"
#include "Actor/Player/DA_Player.h"
#include "Actor/Player/DA_PlayerController.h"
#include "Component/Share/DC_AnimMontage.h"
#include "Actor/DO_AnimInstance.h"
#include "Actor/Player/DA_PlayerState.h"
#include "Component/Share/DC_Skill.h"
#include "Component/Player/DC_Inventory.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

UDC_Stat::UDC_Stat()
{
	PrimaryComponentTick.bCanEverTick = true;

	//UPROPERTY변수 초기화
	characterDataTable = nullptr;

	currentStats = FCharacterStat{};
	currentDataTable = nullptr;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NS_LEVEL_UP(TEXT("/Game/RPGEffects/ParticlesNiagara/StatusEffects/LevelUp/NS_Status_LevelUp"));
	if (NS_LEVEL_UP.Succeeded())
		levelUpEffect = NS_LEVEL_UP.Object;
}

bool UDC_Stat::AddCurrentHP(float _AddHP, AController* _EventInstigator)
{
	DFLOG_N(Display);
	//캐릭터가 죽어있으면 리턴
	if (currentState == ECHARACTER_STATE::DEAD ||
		(currentState == ECHARACTER_STATE::INVINCIBILITY &&
			_AddHP < 0)) return false;

	if (_AddHP < 0) {
		switch (currentState) {
		case ECHARACTER_STATE::CASTING:
		case ECHARACTER_STATE::CHARGING:
		case ECHARACTER_STATE::NORMAL: {
			SetState(ECHARACTER_STATE::HIT);
			montageComp->PlayAnimMontage(TEXT("Hit"));
			break;
		}
		}
	}

	//HP를 추가하고 델리게이트 호출
	currentStats.currentHP += _AddHP;

	if (currentStats.currentHP > currentDataTable->characterHP)
		currentStats.currentHP = currentDataTable->characterHP;

	onChangeHP.Broadcast(currentDataTable->characterHP, currentStats.currentHP);

	//HP가 0보다 낮다면 캐릭터가 죽는다
	if (currentStats.currentHP < KINDA_SMALL_NUMBER) {
		SetState(ECHARACTER_STATE::DEAD);
		GetOwnerCharacter()->GetSkillComponent()->CancelSkill();
		GetOwnerCharacter()->GetSkillComponent()->RemoveAllPassive();
		TArray<FString> montageKeys{};
		TArray<FString> deadAnimKeys{};
		montageComp->GetAnimMontageMap().GenerateKeyArray(montageKeys);
		for (auto montageKey : montageKeys)
			if (montageKey.Contains(TEXT("Dead")))
				deadAnimKeys.Add(montageKey);

		if (!deadAnimKeys.IsEmpty()) {
			FString deadKey = TEXT("Dead") + FString::FromInt((FMath::Rand() % deadAnimKeys.Num()) + 1);
			montageComp->PlayAnimMontage(deadKey);
		}

		if (_EventInstigator == nullptr) return false;

		//죽인 상대가 있다면 그 상대한테 경험치, 골드, 점수를 준다
		auto eventInstigatorPlayer = Cast<ADA_Player>(_EventInstigator->GetPawn());
		DFCHECK(eventInstigatorPlayer != nullptr, false);
		eventInstigatorPlayer->GetStatComponent()->AddEXP(currentDataTable->dropExp);
		eventInstigatorPlayer->GetInventoryComponent()->AddGold(currentDataTable->dropGold);

		auto playerState = _EventInstigator->GetPlayerState<ADA_PlayerState>();
		DFCHECK(playerState != nullptr, false);
		playerState->AddScore(currentDataTable->score);
	}
	return true;
}

void UDC_Stat::SetCurrentHP(float _NewHP)
{
	DFLOG_N(Display);
	currentStats.currentHP = _NewHP;
	onChangeHP.Broadcast(currentDataTable->characterHP, currentStats.currentHP);
}

void UDC_Stat::SetLevel(int32 _NewLevel)
{
	DFLOG_N(Display);
	//레벨이 범위내에 있는지 체크한다
	DFCHECK(MIN_LEVEL <= _NewLevel && _NewLevel <= MAX_LEVEL);


	//데이터 테이블을 가져온다
	currentDataTable = characterDataTable->FindRow<FDFCharacterData>(*FString::FromInt(_NewLevel), TEXT(""));
	DFCHECK(currentDataTable != nullptr);

	if (MIN_LEVEL < _NewLevel &&
		currentStats.characterLevel < _NewLevel &&
		levelUpEffect != nullptr)
		UNiagaraFunctionLibrary::SpawnSystemAttached(levelUpEffect, GetOwnerCharacter()->GetMesh(), TEXT("root"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);

	//가져온 데이터 테이블정보로 스텟을 셋팅해준다
	currentStats.characterLevel = _NewLevel;
	SetCurrentHP(currentDataTable->characterHP);
	CalculateMoveSpeed();

	DFLOG(Warning, TEXT("%s Level : %d"), *GetOwnerCharacter()->GetName(), currentStats.characterLevel);
		
	onChangeDamage.Broadcast(GetDamage(ESTAT_DATA_TYPE::DEFAULT), GetDamage(ESTAT_DATA_TYPE::ADDITION));
	onChangeLevel.Broadcast(currentStats.characterLevel);
}

void UDC_Stat::SetAdditionAttackDamage(const FString& _Key, float _AddAttackDamage)
{
	DFLOG_N(Display);
	//추가공격력을 설정한다 키가 겺친다면 값만 변경된다
	if (currentStats.additionAttckDamage.Contains(_Key))
		currentStats.additionAttckDamage[_Key] = _AddAttackDamage;

	else
		currentStats.additionAttckDamage.Add(TTuple<FString, float>(_Key, _AddAttackDamage));

	onChangeDamage.Broadcast(GetDamage(ESTAT_DATA_TYPE::DEFAULT), GetDamage(ESTAT_DATA_TYPE::ADDITION));
}

void UDC_Stat::SetAdditionMoveSpeed(const FString& _Key, float _AddMoveSpeed)
{
	DFLOG_N(Display);
	//추가이동속도를 설정한다 키가 겺친다면 값만 변경된다
	if (currentStats.additionMoveSpeed.Contains(_Key))
		currentStats.additionMoveSpeed[_Key] = _AddMoveSpeed;

	else
		currentStats.additionMoveSpeed.Add(TTuple<FString, float>(_Key, _AddMoveSpeed));

	CalculateMoveSpeed();
}

float UDC_Stat::GetAdditionAttackDamage(const FString& _Key)
{
	if (currentStats.additionAttckDamage.Contains(_Key))
		return currentStats.additionAttckDamage[_Key];

	else return 0.0f;
}

float UDC_Stat::GetAdditionMoveSpeed(const FString& _Key)
{
	if (currentStats.additionMoveSpeed.Contains(_Key))
		return currentStats.additionMoveSpeed[_Key];

	else return 0.0f;
}

bool UDC_Stat::RemoveAttackDamage(const FString& _Key)
{
	DFLOG_N(Display);
	DFCHECK(currentStats.additionAttckDamage.Contains(_Key), false);
	currentStats.additionAttckDamage.Remove(_Key);
	onChangeDamage.Broadcast(GetDamage(ESTAT_DATA_TYPE::DEFAULT), GetDamage(ESTAT_DATA_TYPE::ADDITION));
	return true;
}

bool UDC_Stat::RemoveMoveSpeed(const FString& _Key)
{
	DFLOG_N(Display);
	DFCHECK(currentStats.additionMoveSpeed.Contains(_Key), false);
	currentStats.additionMoveSpeed.Remove(_Key);
	CalculateMoveSpeed();
	return true;
}

float UDC_Stat::GetDamage(ESTAT_DATA_TYPE _DataType)
{
	float damage{};
	switch (_DataType) {
	case ESTAT_DATA_TYPE::DEFAULT: {
		damage = currentDataTable->attackDamage;
		break;
	}
	case ESTAT_DATA_TYPE::ADDITION: {
		TArray<float> additions{};
		currentStats.additionAttckDamage.GenerateValueArray(additions);
		for (auto d : additions)
			damage += d;
		break;
	}
	case ESTAT_DATA_TYPE::TOTAL: {
		damage = GetDamage(ESTAT_DATA_TYPE::DEFAULT) + GetDamage(ESTAT_DATA_TYPE::ADDITION);
		break;
	}
	}
	return damage;
}

float UDC_Stat::GetSpeed(ESTAT_DATA_TYPE _DataType)
{
	float speed{};
	switch (_DataType) {
	case ESTAT_DATA_TYPE::DEFAULT: {
		speed = currentDataTable->moveSpeed;
		break;
	}
	case ESTAT_DATA_TYPE::ADDITION: {
		TArray<float> additions{};
		currentStats.additionMoveSpeed.GenerateValueArray(additions);
		for (auto s : additions)
			speed += s;
		break;
	}
	case ESTAT_DATA_TYPE::TOTAL: {
		speed = GetSpeed(ESTAT_DATA_TYPE::DEFAULT) + GetSpeed(ESTAT_DATA_TYPE::ADDITION);
		break;
	}
	}
	return speed;
}

void UDC_Stat::SetEXP(int32 _NewEXP)
{
	DFLOG_N(Display);
	currentStats.currentExp = _NewEXP;
	onChangeEXP.Broadcast(currentDataTable->nextExp, currentStats.currentExp);
}

void UDC_Stat::AddEXP(int32 _AddEXP)
{
	DFLOG_N(Display);
	//최대레벨을 이상이라면 리턴한다
	if (currentStats.characterLevel >= MAX_LEVEL) return;

	//현재 경험치에 추가해준다
	currentStats.currentExp += _AddEXP;

	//현재 경험치가 필요경험치를 넘어섰다면 레벨업한다
	while (currentDataTable->nextExp > 0 &&
		currentDataTable->nextExp <= currentStats.currentExp) {
		currentStats.currentExp -= currentDataTable->nextExp;
		SetLevel(FMath::Clamp(currentStats.characterLevel + 1, MIN_LEVEL, MAX_LEVEL));
	}

	onChangeEXP.Broadcast(currentDataTable->nextExp, currentStats.currentExp);
}

void UDC_Stat::AddStunTime(float _StunTime)
{
	switch (currentState)
	{
	case ECHARACTER_STATE::NORMAL:
	case ECHARACTER_STATE::CASTING:
	case ECHARACTER_STATE::CHARGING:
	case ECHARACTER_STATE::STUN:
	case ECHARACTER_STATE::HIT:
		sturnTime += _StunTime;

		if (sturnTime > 0.f) {
			SetState(ECHARACTER_STATE::STUN);
			montageComp->PlayAnimMontage(TEXT("Stun"));
		}
		break;
	}
}

void UDC_Stat::SetState(ECHARACTER_STATE _NewState)
{
	currentState = _NewState;
	onChangeState.Broadcast(currentState);
}

void UDC_Stat::BeginPlay()
{
	Super::BeginPlay();

	montageComp = GetOwnerCharacter()->GetAnimMontageComponent();
	DFCHECK(montageComp != nullptr);
	montageComp->onAnimInstanceChange.AddDynamic(this, &UDC_Stat::AnimInstanceChange);
}

void UDC_Stat::TickComponent(float _DeltaTime, ELevelTick _TickType, FActorComponentTickFunction* _ThisTickFunction)
{
	Super::TickComponent(_DeltaTime, _TickType, _ThisTickFunction);

	switch (currentState)
	{
	case ECHARACTER_STATE::NORMAL:
		if (GetOwnerCharacter()->IsPlayerControlled()) {
			PRINTSTR(PLAYER_STATE, 1.f, FColor::Blue, TEXT("Normal"));
		}
		break;
	case ECHARACTER_STATE::CASTING:
		if (GetOwnerCharacter()->IsPlayerControlled()) {
			PRINTSTR(PLAYER_STATE, 1.f, FColor::Blue, TEXT("Casting"));
		}
		break;
	case ECHARACTER_STATE::CHARGING:
		if (GetOwnerCharacter()->IsPlayerControlled()) {
			PRINTSTR(PLAYER_STATE, 1.f, FColor::Blue, TEXT("Charging"));
		}
		break;
	case ECHARACTER_STATE::STUN: {
		if (GetOwnerCharacter()->IsPlayerControlled()) {
			PRINTSTR(PLAYER_STATE, 1.f, FColor::Blue, TEXT("Stun"));
		}
		sturnTime -= _DeltaTime;
		if (sturnTime <= 0.f) {
			sturnTime = 0.f;
			montageComp->JumpToMontageSection(TEXT("StunEnd"));
			SetState(ECHARACTER_STATE::NORMAL);
		}
		break;
	}
	case ECHARACTER_STATE::HIT:
		if (GetOwnerCharacter()->IsPlayerControlled()) {
			PRINTSTR(PLAYER_STATE, 1.f, FColor::Blue, TEXT("Hit"));
		}
		break;
	case ECHARACTER_STATE::IGNORE:
		if (GetOwnerCharacter()->IsPlayerControlled()) {
			PRINTSTR(PLAYER_STATE, 1.f, FColor::Blue, TEXT("Ignore"));
		}
		break;
	case ECHARACTER_STATE::INVINCIBILITY:
		if (GetOwnerCharacter()->IsPlayerControlled()) {
			PRINTSTR(PLAYER_STATE, 1.f, FColor::Blue, TEXT("Invincibility"));
		}
		break;
	case ECHARACTER_STATE::DEAD:
		if (GetOwnerCharacter()->IsPlayerControlled()) {
			PRINTSTR(PLAYER_STATE, 1.f, FColor::Blue, TEXT("Dead"));
		}
		break;
	}
}

void UDC_Stat::CalculateMoveSpeed()
{
	DFLOG_N(Display);
	GetOwnerCharacter()->GetCharacterMovement()->MaxWalkSpeed = GetSpeed(ESTAT_DATA_TYPE::TOTAL);
	DFLOG(Display, TEXT("%s Total Attack Move Speed : %.2f"), *GetOwnerCharacter()->GetName(), GetOwnerCharacter()->GetCharacterMovement()->MaxWalkSpeed);
	onChangeSpeed.Broadcast(GetSpeed(ESTAT_DATA_TYPE::DEFAULT), GetSpeed(ESTAT_DATA_TYPE::ADDITION));
}

void UDC_Stat::AnimInstanceChange(UDO_AnimInstance* _AnimInstance)
{
	DFCHECK(_AnimInstance != nullptr);
	if (currentAnimInstance != nullptr) {
		currentAnimInstance->onHitEnd.RemoveDynamic(this, &UDC_Stat::HitEnd);
		currentAnimInstance->onDeadEnd.RemoveDynamic(this, &UDC_Stat::DeadEnd);
	}
	currentAnimInstance = _AnimInstance;
	currentAnimInstance->onHitEnd.AddDynamic(this, &UDC_Stat::HitEnd);
	currentAnimInstance->onDeadEnd.AddDynamic(this, &UDC_Stat::DeadEnd);
}

void UDC_Stat::HitEnd()
{
	if (currentState != ECHARACTER_STATE::DEAD)
		SetState(ECHARACTER_STATE::NORMAL);
}

void UDC_Stat::DeadEnd()
{
	montageComp->PauseAnimMontage();
}
