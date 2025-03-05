#include "Component/Share/DC_Skill.h"
#include "Skills/Active/DO_ActiveBase.h"
#include "Skills/Passive/DO_PassiveBase.h"
#include "Actor/DA_Character.h"
#include "Component/Share/DC_AnimMontage.h"
#include "Actor/DO_AnimInstance.h"
#include "Component/Share/DC_Stat.h"
#include "Actor/Player/DOA_Player.h"
#include "Actor/Player/DA_Player.h"
#include "Actor/Monster/AI/DA_AIController.h"

#include "BehaviorTree/BlackboardComponent.h"

UDC_Skill::UDC_Skill()
{
}

void UDC_Skill::SkillRegistration(FKey _Key)
{
	//누른 키에 스킬이 등록되어있다면 스킬을 발동한다
	//준비동작 애님몽타주가 있다면 재생한다
	switch (statComp->GetState())
	{
	case ECHARACTER_STATE::NORMAL:
	case ECHARACTER_STATE::IGNORE:{
		const FString& key = _Key.GetFName().ToString();
		DFCHECK(activeSkills.Contains(key));

		if (!activeSkills[key]->IsAvailable()) return;

		activationSkill = activeSkills[key];
		if (activationSkill->IsChargingSkill()) {
			bIsLoopPass = false;
			montageComp->PlayAnimMontage(activationSkill->GetCastAction(motionIndex));
		}

		activationSkill->SkillPreparation();
			break;
		}
	}
}

void UDC_Skill::SkillActivation()
{
	//스킬을 발동한다
	//시전동작 애님몽타주가 있다면 애님노티파이에 등록해서 발동한다
	if (activationSkill == nullptr || bCasting) return;
	currentSection = 1;

	activationSkill->ApplyCoolDown();
	bCasting = true;

	if (activationSkill->IsChargingSkill())
		bIsLoopPass = true;

	else {
		auto castAction = activationSkill->GetCastAction(motionIndex);
		if (castAction != nullptr)
			montageComp->PlayAnimMontage(castAction);

		else
			UseSkill();
	}
}

void UDC_Skill::ApplyPassive()
{
	for (auto passiveSkill : passiveSkills)
		passiveSkill.Value->RegistPassive(GetOwnerCharacter());
}

void UDC_Skill::UseSkill()
{
	//스킬을 사용한다
	if (activationSkill == nullptr) return;

	activationSkill->UseSkill();

	if (!activationSkill->IsSkillWithAnimation()) {
		statComp->SetState(ECHARACTER_STATE::NORMAL);
		activationSkill = nullptr;
		bCasting = false;
	}
}

void UDC_Skill::EndSkill()
{
	if (activationSkill == nullptr) return;

	bCasting = false;
	onSkillEnd.Broadcast();
	activationSkill->EndSkill();
	statComp->SetState(ECHARACTER_STATE::NORMAL);
	activationSkill = nullptr;
}

void UDC_Skill::ComboAttack()
{
	auto monsterController = Cast<ADA_AIController>(GetOwnerCharacter()->GetController());
	if (monsterController != nullptr) {
		auto canAttack = monsterController->GetBlackboardComponent()->GetValueAsBool(ADA_AIController::canAttackKey);
		if (!canAttack) {
			EndSkill();
			return;
		}
	}

	switch (statComp->GetState())
	{
	case ECHARACTER_STATE::DEAD:
	case ECHARACTER_STATE::STUN:
	case ECHARACTER_STATE::HIT: {
		EndSkill();
		return;
	}
	}

	FName prevSection = *FString::Printf(TEXT("Attack%d"), currentSection);
	FName nextSection = *FString::Printf(TEXT("Attack%d"), ++currentSection);
	montageComp->SetNextSection(prevSection, nextSection);
}

void UDC_Skill::SkillNotify()
{
	DFCHECK(activationSkill != nullptr);
	activationSkill->SkillNotify();
}

bool UDC_Skill::AddActiveSkillbyClass(const FString& _Key, TSubclassOf<UDO_ActiveBase> _SkillClass)
{
	DFLOG_N(Display);
	auto skill = NewObject<UDO_ActiveBase>(this, _SkillClass);
	DFCHECK(skill != nullptr, false);
	skill->SetOwnerCharacter(GetOwnerCharacter());
	activeSkills.Add(_Key, skill);
	onActiveChange.Broadcast(_Key, skill, true);
	return true;
}

bool UDC_Skill::AddActiveSkillbyName(const FString& _Key, const FString& _SkillName)
{
	auto activeData = activeSkillDataTable->FindRow<FDFActiveSkillData>(*_SkillName, TEXT(""));
	DFCHECK(activeData != nullptr, false);
	return AddActiveSkillbyClass(_Key, activeData->activeSkillClass);
}

bool UDC_Skill::RemoveActiveSkill(const FString& _Key)
{
	DFLOG_N(Display);
	DFCHECK(activeSkills.Contains(_Key), false);
	onActiveChange.Broadcast(_Key, activeSkills[_Key], false);
	return activeSkills.Remove(_Key) > 0;
}

bool UDC_Skill::AddPassiveSkillbyClass(const FString& _Key, TSubclassOf<UDO_PassiveBase> _SkillClass)
{
	DFLOG_N(Display);
	auto skill = NewObject<UDO_PassiveBase>(this, _SkillClass);
	DFCHECK(skill != nullptr, false);
	skill->RegistPassive(GetOwnerCharacter());
	passiveSkills.Add(_Key, skill);
	onPassiveChange.Broadcast(_Key, skill, true);
	return true;
}

bool UDC_Skill::AddPassiveSkillbyName(const FString& _Key, const FString& _SkillName)
{
	auto passiveData = passiveSkillDataTable->FindRow<FDFPassiveSkillData>(*_SkillName, TEXT(""));
	DFCHECK(passiveData != nullptr, false);
	return AddPassiveSkillbyClass(_Key, passiveData->passiveSkillClass);
}

bool UDC_Skill::RemovePassiveSkill(const FString& _Key)
{
	DFLOG_N(Display);
	DFCHECK(passiveSkills.Contains(_Key), false);
	onPassiveChange.Broadcast(_Key, passiveSkills[_Key], false);
	passiveSkills[_Key]->RemovePassive();
	return passiveSkills.Remove(_Key) > 0;
}

void UDC_Skill::RemoveAllActive()
{
	TArray<FString> keys{};
	activeSkills.GenerateKeyArray(keys);

	for (auto key : keys)
		RemoveActiveSkill(key);
}

void UDC_Skill::RemoveAllPassive()
{
	TArray<FString> keys{};
	passiveSkills.GenerateKeyArray(keys);

	for (auto key : keys)
		RemovePassiveSkill(key);
}

UDO_ActiveBase* UDC_Skill::GetActiveSkill(const FString& _Key) const
{
	DFCHECK(activeSkills.Contains(_Key), nullptr);
	return activeSkills[_Key];
}

UDO_PassiveBase* UDC_Skill::GetPassiveSkill(const FString& _Key) const
{
	DFCHECK(passiveSkills.Contains(_Key), nullptr);
	return passiveSkills[_Key];
}

void UDC_Skill::SetMotionIndex(int32 _Index)
{
	motionIndex = _Index;
}

void UDC_Skill::CancelSkill()
{
	if (activationSkill != nullptr) {
		bCasting = false;
		montageComp->StopAnimMontage();
		activationSkill->CancelSkill();
		activationSkill = nullptr;
	}
}

void UDC_Skill::BeginPlay()
{
	Super::BeginPlay();

	montageComp = GetOwnerCharacter()->GetAnimMontageComponent();
	statComp = GetOwnerCharacter()->GetStatComponent();
	AnimInstanceChange(montageComp->GetOwnerAnimInstance());

	DFCHECK(montageComp != nullptr);
	montageComp->onAnimInstanceChange.AddDynamic(this, &UDC_Skill::AnimInstanceChange);
	DFCHECK(statComp != nullptr);
	statComp->onChangeState.AddDynamic(this, &UDC_Skill::ChangedState);
}

void UDC_Skill::AnimInstanceChange(UDO_AnimInstance* _AnimInstance)
{
	DFCHECK(_AnimInstance != nullptr);
	if (currentAnimInstance != nullptr) {
		currentAnimInstance->onUseSkill.RemoveDynamic(this, &UDC_Skill::UseSkill);
		currentAnimInstance->onCastingStart.RemoveDynamic(this, &UDC_Skill::CastingStart);
		currentAnimInstance->onSkillLoopCheck.RemoveDynamic(this, &UDC_Skill::LoopCheck);
		currentAnimInstance->onEndSkill.RemoveDynamic(this, &UDC_Skill::EndSkill);
		currentAnimInstance->onComboAttack.RemoveDynamic(this, &UDC_Skill::ComboAttack);
		currentAnimInstance->onSkillNotify.RemoveDynamic(this, &UDC_Skill::SkillNotify);
	}
	currentAnimInstance = _AnimInstance;
	currentAnimInstance->onUseSkill.AddDynamic(this, &UDC_Skill::UseSkill);
	currentAnimInstance->onCastingStart.AddDynamic(this, &UDC_Skill::CastingStart);
	currentAnimInstance->onSkillLoopCheck.AddDynamic(this, &UDC_Skill::LoopCheck);
	currentAnimInstance->onEndSkill.AddDynamic(this, &UDC_Skill::EndSkill);
	currentAnimInstance->onComboAttack.AddDynamic(this, &UDC_Skill::ComboAttack);
	currentAnimInstance->onSkillNotify.AddDynamic(this, &UDC_Skill::SkillNotify);
}

void UDC_Skill::ChangedState(ECHARACTER_STATE _NewState)
{
	switch (_NewState)
	{
	case ECHARACTER_STATE::STUN:
	case ECHARACTER_STATE::DEAD:
	case ECHARACTER_STATE::HIT:
		if (activationSkill != nullptr) {
			bCasting = false;
			activationSkill->CancelSkill();
			activationSkill = nullptr;
		}
		break;
	}
}

void UDC_Skill::CastingStart()
{
	if (activationSkill != nullptr)
		statComp->SetState(activationSkill->GetCastState());
}

void UDC_Skill::LoopCheck()
{
	if (bIsLoopPass)
		montageComp->SetNextSection(TEXT("Loop"), TEXT("End"));
}
