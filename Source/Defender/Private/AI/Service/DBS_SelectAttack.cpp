#include "AI/Service/DBS_SelectAttack.h"
#include "Actor/Monster/AI/DA_AIController.h"
#include "Actor/Monster/DA_Monster.h"
#include "Component/Share/DC_Skill.h"
#include "Skills/Active/DO_ActiveBase.h"

#include "BehaviorTree/BlackboardComponent.h"

UDBS_SelectAttack::UDBS_SelectAttack()
{
	NodeName = TEXT("SelectAttack");
	Interval = 0.1f;
	RandomDeviation = 0.f;
}

void UDBS_SelectAttack::TickNode(UBehaviorTreeComponent& _OwnerComp, uint8* _NodeMemory, float _DeltaSeconds)
{
	Super::TickNode(_OwnerComp, _NodeMemory, _DeltaSeconds);

	//공격할 다음 스킬을 설정합니다
	auto aiController = Cast<ADA_AIController>(_OwnerComp.GetAIOwner());

	if (aiController == nullptr) return;

	if (aiController->GetTraceState() == EMONSTER_TRACE_STATE::ATTACK) return;

	auto monster = aiController->GetControlledMonster();

	if (monster == nullptr) return;

	auto skillComp = monster->GetSkillComponent();

	if (skillComp == nullptr) return;

	FString skillName = aiController->GetBlackboardComponent()->GetValueAsString(ADA_AIController::skillNameKey);
	auto skill = skillComp->GetActiveSkill(skillName);

	skillName = aiController->GetRandomSkillName();
	skill = skillComp->GetActiveSkill(skillName);

	if (skill == nullptr || !skill->IsAvailable()) return;

	aiController->GetBlackboardComponent()->SetValueAsString(ADA_AIController::skillNameKey, skillName);
}
