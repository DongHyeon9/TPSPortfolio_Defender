#include "AI/Service/DBS_FindTarget.h"
#include "Actor/Monster/AI/DA_AIController.h"
#include "Component/Share/DC_Stat.h"
#include "Actor/Player/DA_Player.h"

#include "BehaviorTree/BlackboardComponent.h"

UDBS_FindTarget::UDBS_FindTarget()
{
	NodeName = TEXT("FindTarget");
	Interval = 10.f;
	RandomDeviation = 5.f;
}

void UDBS_FindTarget::TickNode(UBehaviorTreeComponent& _OwnerComp, uint8* _NodeMemory, float _DeltaSeconds)
{
	Super::TickNode(_OwnerComp, _NodeMemory, _DeltaSeconds);
	//입장해있는 플레이어들중 살아있는 플레이어를 식별해서 타겟으로 설정한다
	auto aiController = Cast<ADA_AIController>(_OwnerComp.GetAIOwner());
	if (aiController == nullptr) return;

	auto blackBoardComp = aiController->GetBlackboardComponent();

	blackBoardComp->SetValueAsBool(ADA_AIController::canAttackKey, true);

	auto currentTarget = blackBoardComp->GetValueAsObject(ADA_AIController::targetKey);

	for (FConstPlayerControllerIterator pIter = GetWorld()->GetPlayerControllerIterator(); pIter; ++pIter) {
		auto playerCharacter = Cast<ADA_Player>(pIter->Get()->GetPawn());

		if (playerCharacter == nullptr ||
			playerCharacter->GetStatComponent()->GetState() == ECHARACTER_STATE::DEAD)
			continue;

		if (currentTarget == nullptr ||
			currentTarget != playerCharacter) {
			blackBoardComp->SetValueAsObject(ADA_AIController::targetKey, playerCharacter);
			break;
		}
	}
	currentTarget = blackBoardComp->GetValueAsObject(ADA_AIController::targetKey);
	DFCHECK(currentTarget != nullptr);
}
