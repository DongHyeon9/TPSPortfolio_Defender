#include "Widget/InGame/Information/DWP_SkillSlot.h"
#include "Skills/Active/DO_ActiveBase.h"

#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

UDWP_SkillSlot::UDWP_SkillSlot(const FObjectInitializer& _ObjectInitializer):
	Super(_ObjectInitializer)
{
}

void UDWP_SkillSlot::SetSkillKey(const FString& _Key)
{
	key = _Key;
}

void UDWP_SkillSlot::SetSkillKeyText(const FString& _Key)
{
	txt_SkillKey->SetText(FText::FromString(_Key));
}

void UDWP_SkillSlot::SetSkillImage(UTexture2D* _SkillImage)
{
	img_Skill->SetBrushFromTexture(_SkillImage);
}

void UDWP_SkillSlot::SetActiveSkillChange(const FString& _Key, UDO_ActiveBase* _Skill, bool _bIsRegist)
{
	DFLOG_N(Display);

	//스킬을 등록해준다
	if (_bIsRegist) {
		if (key != _Key) return;

		currentSkill = _Skill;
		currentSkill->onCoolDown.RemoveDynamic(this, &UDWP_SkillSlot::SetSkillProgress);
		currentSkill->onCoolDown.AddDynamic(this, &UDWP_SkillSlot::SetSkillProgress);
		img_Skill->SetBrushFromTexture(_Skill->GetSkillIcon());
	}

	//등록된 스킬을 삭제한다
	else {
		key = FString();
		currentSkill->onCoolDown.RemoveDynamic(this, &UDWP_SkillSlot::SetSkillProgress);
		img_Skill->SetBrushFromTexture(nullptr);
		currentSkill = nullptr;
	}
}

void UDWP_SkillSlot::NativeDestruct()
{
	Super::NativeDestruct();

	if (currentSkill != nullptr)
		currentSkill->onCoolDown.RemoveDynamic(this, &UDWP_SkillSlot::SetSkillProgress);
}

void UDWP_SkillSlot::SetSkillProgress(float _Total, float _Current)
{
	//스킬의 프로그레스 바를 설정해준다
	pb_CoolDown->SetPercent(_Current / _Total);
	if (_Current > 0.f)
		txt_SkillCooldown->SetText(FText::AsNumber((int32)_Current));
	else
		txt_SkillCooldown->SetText(FText::FromString(TEXT("")));
}
