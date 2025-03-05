#include "AI/Task/DBT_TurnToTarget.h"
#include "Actor/DA_Character.h"
#include "Actor/Monster/AI/DA_AIController.h"

#include "BehaviorTree/BlackboardComponent.h"

UDBT_TurnToTarget::UDBT_TurnToTarget()
{
	NodeName = TEXT("Turn");
	turnSpeed = 5.f;
}

EBTNodeResult::Type UDBT_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& _OwnerComp, uint8* _NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(_OwnerComp, _NodeMemory);
	//몬스터를 공격하고 있는 플레이어 방향으로 회전합니다
	auto monster = Cast<ADA_Character>(_OwnerComp.GetAIOwner()->GetPawn());
	auto target = Cast<ADA_Character>(_OwnerComp.GetBlackboardComponent()->GetValueAsObject(ADA_AIController::targetKey));
	if (!monster || !target) return EBTNodeResult::Failed;

	FVector lookVector = target->GetActorLocation() - monster->GetActorLocation();
	lookVector.Z = 0.f;
	FRotator targetRot = FRotationMatrix::MakeFromX(lookVector).Rotator();
	monster->SetActorRotation(FMath::RInterpTo(monster->GetActorRotation(), targetRot, GetWorld()->GetDeltaSeconds(), turnSpeed));

	return EBTNodeResult::Succeeded;
}
