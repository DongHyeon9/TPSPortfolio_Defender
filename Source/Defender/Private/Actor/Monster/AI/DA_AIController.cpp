#include "Actor/Monster/AI/DA_AIController.h"
#include "Actor/Monster/DA_Monster.h"
#include "Component/Share/DC_Stat.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "Kismet/KismetMathLibrary.h"

const FName ADA_AIController::patrolPosKey(TEXT("PatrolPos"));
const FName ADA_AIController::targetKey(TEXT("Target"));
const FName ADA_AIController::skillNameKey(TEXT("SkillName"));
const FName ADA_AIController::canAttackKey(TEXT("CanAttack"));

ADA_AIController::ADA_AIController()
{
	aiPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));

	damageSense = CreateOptionalDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage"));
	sightSense = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));
	hearingSense = CreateOptionalDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing"));

	sightSense->DetectionByAffiliation.bDetectEnemies = true;
	sightSense->DetectionByAffiliation.bDetectFriendlies = true;
	sightSense->DetectionByAffiliation.bDetectNeutrals = true;

	hearingSense->DetectionByAffiliation.bDetectEnemies = true;
	hearingSense->DetectionByAffiliation.bDetectFriendlies = true;
	hearingSense->DetectionByAffiliation.bDetectNeutrals = true;

	damageSense->SetMaxAge(3.f);

	aiPerceptionComp->ConfigureSense(*sightSense);
	aiPerceptionComp->ConfigureSense(*hearingSense);
	aiPerceptionComp->ConfigureSense(*damageSense);
}

void ADA_AIController::SetTraceState(EMONSTER_TRACE_STATE _TraceState)
{
	//현재 상태에따라 이속을 설정합니다
	//추격상태면 이동속도가 증가합니다
	currentTraceState = _TraceState;
	switch (currentTraceState)
	{
	case EMONSTER_TRACE_STATE::IDLE:
	case EMONSTER_TRACE_STATE::PATROL:{
		controlledMonster->GetStatComponent()->RemoveMoveSpeed(TEXT("TraceState"));
		break;
	}
	case EMONSTER_TRACE_STATE::TRACE: {
		controlledMonster->GetStatComponent()->SetAdditionMoveSpeed(
			TEXT("TraceState"),
			controlledMonster->GetStatComponent()->GetSpeed());
		break;
	}
	}
	onTraceStateChange.Broadcast(currentTraceState);
}

void ADA_AIController::RunAI()
{
	//AI를 다시 시작합니다
	if (!BrainComponent->IsRunning())
		BrainComponent->RestartLogic();

	//마지막 타겟과 마지막 정찰 위치를 복원합니다
	SetTraceState(EMONSTER_TRACE_STATE::IDLE);
	GetBlackboardComponent()->SetValueAsObject(targetKey, targetPlayer);
	GetBlackboardComponent()->SetValueAsVector(patrolPosKey, patrolTemp);
}

void ADA_AIController::StopAI(const FString& _Reason)
{
	//현재 타겟과 정찰위치를 저장합니다
	SetTraceState(EMONSTER_TRACE_STATE::IDLE);
	targetPlayer = GetBlackboardComponent()->GetValueAsObject(targetKey);
	patrolTemp = GetBlackboardComponent()->GetValueAsVector(patrolPosKey);

	//AI를 정지합니다
	if (BrainComponent->IsRunning())
		BrainComponent->StopLogic(_Reason);
}

void ADA_AIController::InitializeAIController(const TArray<FDFActiveSkillData>& _MonsterSkillData)
{
	//스킬 사정거리 맵을 설정합니다
	skillRangeMap.Reset();
	for (auto skill : _MonsterSkillData)
		skillRangeMap.Add(skill.monsterSkillKey, skill.attackRange);
}

bool ADA_AIController::GetSkillRange(const FString& _SkillName, float& _OutRange)
{
	//스킬의 사정거리를 가져온다
	DFCHECK(skillRangeMap.Contains(_SkillName), false);
	_OutRange = skillRangeMap[_SkillName];
	return true;
}

FString ADA_AIController::GetRandomSkillName()
{
	//랜덤으로 스킬이름을 가져옵니다
	TArray<FString> keyNames{};
	skillRangeMap.GenerateKeyArray(keyNames);
	if (keyNames.IsEmpty()) return FString();
	int32 skillIndex = FMath::Rand() % keyNames.Num();
	return keyNames[skillIndex];
}

void ADA_AIController::OnPossess(APawn* _InPawn)
{
	Super::OnPossess(_InPawn);

	aiPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &ADA_AIController::OnUpdatedPerception);
	controlledMonster = Cast<ADA_Monster>(_InPawn);
	DFCHECK(controlledMonster != nullptr);
	IDI_ObjectPoolingActor::Execute_ActiveSetting(controlledMonster, false);
	controlledMonster->GetStatComponent()->onChangeState.AddDynamic(this, &ADA_AIController::OnChangeState);
}

void ADA_AIController::OnUpdatedPerception(AActor* _Actor, FAIStimulus _Stimulus)
{
	//몬스터가 활성화되어있다면 감지된 자극에 따라 행동을 다르게한다
	if (!controlledMonster->IsActive()) return;

	auto character = Cast<ADA_Character>(_Actor);
	if (character == nullptr || !character->IsPlayerControlled()) return;
	
	bool bIsSensed = _Stimulus.WasSuccessfullySensed();

	auto senseClass = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), _Stimulus);

	auto blackBoard = GetBlackboardComponent();
	if (blackBoard == nullptr) return;

	//시야에 들어오면 타겟을 셋팅해준다
	if (senseClass == UAISense_Sight::StaticClass()) {
		if (bIsSensed) {
			blackBoard->SetValueAsObject(targetKey, _Actor);
			targetPlayer = _Actor;
			SetTraceState(EMONSTER_TRACE_STATE::SIGHT);
		}
		else {
			blackBoard->SetValueAsObject(targetKey, nullptr);
			targetPlayer = nullptr;
			SetTraceState(EMONSTER_TRACE_STATE::IDLE);
		}
	}

	//소리를 들으면 소리가 났던 방향으로 움직입니다
	else if (senseClass == UAISense_Hearing::StaticClass() &&
		bIsSensed) {
		switch (currentCharacterState)
		{
		case ECHARACTER_STATE::NORMAL:
		case ECHARACTER_STATE::INVINCIBILITY:
		case ECHARACTER_STATE::CASTING:
		case ECHARACTER_STATE::CHARGING:
		case ECHARACTER_STATE::IGNORE: {
			blackBoard->SetValueAsVector(patrolPosKey, _Stimulus.StimulusLocation);
			break;
		}
		case ECHARACTER_STATE::STUN:
		case ECHARACTER_STATE::HIT: {
			patrolTemp = _Stimulus.StimulusLocation;
			break;
		}
		}
		SetTraceState(EMONSTER_TRACE_STATE::HEARING);
	}

	//데미지를 받으면 데미지가 들어온 방향을 쳐다본다
	else if (senseClass == UAISense_Damage::StaticClass() &&
		IsComparisonHigherState(EMONSTER_TRACE_STATE::DAMAGE) &&
		bIsSensed) {
		FVector targetDirection = _Stimulus.StimulusLocation - _Stimulus.ReceiverLocation;
		targetDirection.Normalize();

		float lookAngle = UKismetMathLibrary::FindLookAtRotation(_Stimulus.ReceiverLocation, _Stimulus.StimulusLocation).Yaw + 180.f;
		float controlAngle = GetControlRotation().Yaw + 180.f;
		float targetAngle = UKismetMathLibrary::Abs(lookAngle - controlAngle);
		float moveScalar = UKismetMathLibrary::MapRangeClamped(targetAngle, 0.f, 180.f, 50.f, 100.f);

		FVector patrolPos = (targetDirection * moveScalar) + _Stimulus.ReceiverLocation;

		blackBoard->SetValueAsVector(patrolPosKey, patrolPos);
		SetTraceState(EMONSTER_TRACE_STATE::DAMAGE);
	}
}

void ADA_AIController::OnChangeState(ECHARACTER_STATE _CurrentState)
{
	currentCharacterState = _CurrentState;

	switch (currentCharacterState)
	{
	case ECHARACTER_STATE::STUN: {
		StopAI("Stun");
		break;
	}
	case ECHARACTER_STATE::DEAD: {
		StopAI("Dead");
		break;
	}
	}
}
