#include "AI/Task/DBT_FindPatrolPos.h"
#include "Actor/Monster/AI/DA_AIController.h"
#include "Actor/Monster/DA_Monster.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UDBT_FindPatrolPos::UDBT_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
	patrolRadius = 500.f;
}

EBTNodeResult::Type UDBT_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& _OwnerComp, uint8* _NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(_OwnerComp, _NodeMemory);
	//정찰할 다음 위치를 설정합니다
	auto aiController = Cast<ADA_AIController>(_OwnerComp.GetAIOwner());
	if (aiController == nullptr) return EBTNodeResult::Failed;

	auto monster = aiController->GetControlledMonster();
	if (monster == nullptr) return EBTNodeResult::Failed;

	if (aiController->GetTraceState() != EMONSTER_TRACE_STATE::IDLE) return EBTNodeResult::Succeeded;

	UNavigationSystemV1* navSystem = UNavigationSystemV1::GetNavigationSystem(monster->GetWorld());
	if (navSystem == nullptr) return EBTNodeResult::Failed;

	FNavLocation nextPatrolLocation{};

	if (navSystem->GetRandomPointInNavigableRadius(monster->GetActorLocation(), patrolRadius, nextPatrolLocation)) {
		_OwnerComp.GetBlackboardComponent()->SetValueAsVector(ADA_AIController::patrolPosKey, nextPatrolLocation.Location);
		return EBTNodeResult::Succeeded;
	}
	return result;
}
