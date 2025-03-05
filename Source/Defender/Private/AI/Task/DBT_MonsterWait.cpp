#include "AI/Task/DBT_MonsterWait.h"
#include "Actor/Monster/AI/DA_AIController.h"

UDBT_MonsterWait::UDBT_MonsterWait()
{
	NodeName = TEXT("MonsterWait");
}

EBTNodeResult::Type UDBT_MonsterWait::ExecuteTask(UBehaviorTreeComponent& _OwnerComp, uint8* _NodeMemory)
{
	auto result = Super::ExecuteTask(_OwnerComp, _NodeMemory);
	//몬스터가 일정시간동안 대기합니다
	ownerComp = &_OwnerComp;

	if (ownerComp == nullptr) return EBTNodeResult::Failed;

	aiController = Cast<ADA_AIController>(ownerComp->GetAIOwner());

	if (aiController == nullptr) return EBTNodeResult::Failed;

	switch (aiController->GetTraceState()) {
	case EMONSTER_TRACE_STATE::DAMAGE:
	case EMONSTER_TRACE_STATE::HEARING:
	case EMONSTER_TRACE_STATE::SIGHT: {
		return EBTNodeResult::Succeeded;
	}
	}

	aiController->SetTraceState(EMONSTER_TRACE_STATE::IDLE);
	aiController->onTraceStateChange.RemoveDynamic(this, &UDBT_MonsterWait::OnChangeTraceState);
	aiController->onTraceStateChange.AddDynamic(this, &UDBT_MonsterWait::OnChangeTraceState);

	return result;
}

void UDBT_MonsterWait::OnChangeTraceState(EMONSTER_TRACE_STATE _NewState)
{
	switch (_NewState)
	{
	case EMONSTER_TRACE_STATE::DAMAGE:
	case EMONSTER_TRACE_STATE::HEARING:
	case EMONSTER_TRACE_STATE::SIGHT: {
		FinishLatentTask(*ownerComp, EBTNodeResult::Succeeded);
		break;
	}
	}
}
