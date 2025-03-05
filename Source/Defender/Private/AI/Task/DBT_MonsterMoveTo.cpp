#include "AI/Task/DBT_MonsterMoveTo.h"
#include "Actor/Monster/AI/DA_AIController.h"
#include "Actor/Monster/DA_Monster.h"
#include "Component/Share/DC_Stat.h"

#include "BehaviorTree/BlackboardComponent.h"

UDBT_MonsterMoveTo::UDBT_MonsterMoveTo()
{
	NodeName = TEXT("MonsterMoveTo");
}

EBTNodeResult::Type UDBT_MonsterMoveTo::ExecuteTask(UBehaviorTreeComponent& _OwnerComp, uint8* _NodeMemory)
{
	auto result = Super::ExecuteTask(_OwnerComp, _NodeMemory);
	//몬스터를 지정된 위치까지 움직입니다
	auto aiController = Cast<ADA_AIController>(_OwnerComp.GetAIOwner());
	if (aiController == nullptr) return EBTNodeResult::Failed;

	auto monster = aiController->GetControlledMonster();

	if (aiController->GetTraceState() == EMONSTER_TRACE_STATE::ATTACK ||
		monster->GetStatComponent()->GetState() == ECHARACTER_STATE::HIT)
		return EBTNodeResult::Failed;
	
	aiController->SetTraceState(traceState);

	return result;
}
