#include "AI/Task/DBT_Attack.h"
#include "Actor/Monster/AI/DA_AIController.h"
#include "Actor/Monster/DA_Monster.h"
#include "Component/Share/DC_Skill.h"
#include "Actor/DO_AnimInstance.h"
#include "Skills/Active/DO_ActiveBase.h"

#include "BehaviorTree/BlackboardComponent.h"

UDBT_Attack::UDBT_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UDBT_Attack::ExecuteTask(UBehaviorTreeComponent& _OwnerComp, uint8* _NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(_OwnerComp, _NodeMemory);
	ownerComp = &_OwnerComp;
	//공격이 가능한 상태라면 공격을 실행한다
	aiController = Cast<ADA_AIController>(ownerComp->GetAIOwner());
	if (aiController == nullptr) return EBTNodeResult::Failed;

	auto monster = aiController->GetControlledMonster();
	if (monster == nullptr) return EBTNodeResult::Failed;

	auto animInstance = Cast<UDO_AnimInstance>(monster->GetMesh()->GetAnimInstance());
	animInstance->onEndSkill.RemoveDynamic(this, &UDBT_Attack::OnEndSkill);
	animInstance->onEndSkill.AddDynamic(this, &UDBT_Attack::OnEndSkill);

	auto skillComp = monster->GetSkillComponent();
	if (skillComp == nullptr) return EBTNodeResult::Failed;

	FString skillName = aiController->GetBlackboardComponent()->GetValueAsString(ADA_AIController::skillNameKey);
	bool bCanAttack = aiController->GetBlackboardComponent()->GetValueAsBool(ADA_AIController::canAttackKey);

	skillComp->onSkillEnd.RemoveDynamic(this, &UDBT_Attack::OnEndSkill);
	skillComp->onSkillEnd.AddDynamic(this, &UDBT_Attack::OnEndSkill);

	auto skill = skillComp->GetActiveSkill(skillName);
	if(skill==nullptr) return EBTNodeResult::Failed;

	if (skill->IsAvailable() &&
		bCanAttack &&
		aiController->GetTraceState() != EMONSTER_TRACE_STATE::ATTACK) {
		skillComp->SkillRegistration(*skillName);
		skillComp->SkillActivation();
		aiController->SetTraceState(EMONSTER_TRACE_STATE::ATTACK);
		return EBTNodeResult::InProgress;
	}
	else
		return EBTNodeResult::Failed;
}

void UDBT_Attack::OnEndSkill()
{
	//스킬이 끝났다면 테스크를 종료한다
	aiController->SetTraceState(EMONSTER_TRACE_STATE::IDLE);
	FinishLatentTask(*ownerComp, EBTNodeResult::Succeeded);
}
