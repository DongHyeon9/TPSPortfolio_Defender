#include "Widget/Select/DWP_SelectSkillSlot.h"

#include "Components/CheckBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UDWP_SelectSkillSlot::SetCheckState(bool _bIsCheck)
{
	//스킬의 체크상태를 변경하고 false면 인덱스도 없애줍니다
	if (_bIsCheck)
		cb_Check->SetCheckedState(ECheckBoxState::Checked);
	else {
		txt_Index->SetText(FText::FromString(TEXT("")));
		cb_Check->SetCheckedState(ECheckBoxState::Unchecked);
	}
}

void UDWP_SelectSkillSlot::SetSkillType(bool _bIsActiveSkill)
{
	bIsActiveSkill = _bIsActiveSkill;
}

void UDWP_SelectSkillSlot::SetSkillName(const FString& _SkillName)
{
	skillName = _SkillName;
}

void UDWP_SelectSkillSlot::SetIndex(int32 _Index)
{
	txt_Index->SetText(FText::AsNumber(_Index + 1));
}

void UDWP_SelectSkillSlot::SetSkillImage(UTexture2D* _NewImage)
{
	img_SkillIcon->SetBrushFromTexture(_NewImage);
}

void UDWP_SelectSkillSlot::NativeConstruct()
{
	Super::NativeConstruct();

	cb_Check->OnCheckStateChanged.AddDynamic(this, &UDWP_SelectSkillSlot::OnCheck);
}

void UDWP_SelectSkillSlot::NativeDestruct()
{
	Super::NativeDestruct();

	cb_Check->OnCheckStateChanged.RemoveDynamic(this, &UDWP_SelectSkillSlot::OnCheck);
}

void UDWP_SelectSkillSlot::OnCheck(bool _bIsChecked)
{
	if (!_bIsChecked)
		txt_Index->SetText(FText::FromString(TEXT("")));

	onChecked.Broadcast(this, _bIsChecked);
}
