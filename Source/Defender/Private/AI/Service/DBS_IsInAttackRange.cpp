#include "AI/Service/DBS_IsInAttackRange.h"
#include "Actor/Monster/AI/DA_AIController.h"
#include "Actor/Monster/DA_Monster.h"
#include "Component/Share/DC_Skill.h"
#include "Skills/Active/DO_ActiveBase.h"

#include "BehaviorTree/BlackboardComponent.h"

UDBS_IsInAttackRange::UDBS_IsInAttackRange()
{
	NodeName = TEXT("AttackRangeCheck");
	Interval = 0.1f;
	RandomDeviation = 0.f;
}

void UDBS_IsInAttackRange::TickNode(UBehaviorTreeComponent& _OwnerComp, uint8* _NodeMemory, float _DeltaSeconds)
{
	Super::TickNode(_OwnerComp, _NodeMemory, _DeltaSeconds);

	//현재 타겟이 공격 범위내로 들어왔는지 확인합니다
	auto aiController = Cast<ADA_AIController>(_OwnerComp.GetAIOwner());
	if (aiController == nullptr) return;

	auto blackBoardComp = aiController->GetBlackboardComponent();

	auto monster = aiController->GetControlledMonster();
	if (monster == nullptr) return;

	FString skillName = aiController->GetBlackboardComponent()->GetValueAsString(ADA_AIController::skillNameKey);
	float skillRnage{};
	if (!aiController->GetSkillRange(skillName, skillRnage)) return;

	auto target = Cast<ADA_Character>(aiController->GetBlackboardComponent()->GetValueAsObject(ADA_AIController::targetKey));
	if (target == nullptr) return;

	auto skillComp = monster->GetSkillComponent();
	if (skillComp == nullptr) return;

	auto skill = skillComp->GetActiveSkill(skillName);

	float currentDistance = target->GetDistanceTo(monster);

	blackBoardComp->SetValueAsBool(ADA_AIController::canAttackKey, currentDistance <= skillRnage);
}
