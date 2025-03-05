#include "Widget/Select/DWP_CharacterSelectSlot.h"

#include "Components/Image.h"
#include "Components/CheckBox.h"

void UDWP_CharacterSelectSlot::SetCharacterIndex(int32 _Index)
{
	characterIndex = _Index;
}

void UDWP_CharacterSelectSlot::SetCharacterImage(UTexture2D* _NewImage)
{
	img_Character->SetBrushFromTexture(_NewImage);
}

void UDWP_CharacterSelectSlot::SetCheckState(bool _bIsCheck)
{
	if (_bIsCheck)
		cb_Check->SetCheckedState(ECheckBoxState::Checked);
	else
		cb_Check->SetCheckedState(ECheckBoxState::Unchecked);
}

void UDWP_CharacterSelectSlot::NativeConstruct()
{
	Super::NativeConstruct();

	cb_Check->OnCheckStateChanged.AddDynamic(this, &UDWP_CharacterSelectSlot::OnCheck);
}

void UDWP_CharacterSelectSlot::NativeDestruct()
{
	Super::NativeDestruct();

	cb_Check->OnCheckStateChanged.RemoveDynamic(this, &UDWP_CharacterSelectSlot::OnCheck);
}

void UDWP_CharacterSelectSlot::OnCheck(bool _bIsChecked)
{
	if (_bIsChecked)
		onChecked.Broadcast(this);
}
