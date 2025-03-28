#include "Widget/Select/DWP_StartSkill.h"
#include "Widget/Select/DWP_SelectSkillSlot.h"
#include "Actor/Player/Controller/DAPC_Select.h"
#include "Skills/DO_SkillBase.h"

#include "Components/Button.h"
#include "Blueprint/WidgetTree.h"

namespace START_SKILL_WIDGET
{
	constexpr uint32 ACTIVE_SLOT_COUNT{ 8 };
	constexpr uint32 PASSIVE_SLOT_COUNT{ 3 };
}

UDWP_StartSkill::UDWP_StartSkill(const FObjectInitializer& _ObjectInitializer):
	Super(_ObjectInitializer)
{
}

void UDWP_StartSkill::InitializeWidget()
{
	Super::InitializeWidget();

	//위젯을 바인딩 해준다
	skillSlots.Reset(START_SKILL_WIDGET::ACTIVE_SLOT_COUNT + START_SKILL_WIDGET::PASSIVE_SLOT_COUNT);

	for (uint32 i = 0; i < START_SKILL_WIDGET::ACTIVE_SLOT_COUNT; ++i)
	{
		FName currentSlotName{ FString::Printf(TEXT("WG_SkillSelectSlot_A_%d"), i) };
		skillSlots[i] = CastChecked<UDWP_SelectSkillSlot>(GetWidgetFromName(currentSlotName));
	}

	for (uint32 i = 0; i < START_SKILL_WIDGET::PASSIVE_SLOT_COUNT; ++i)
	{
		FName currentSlotName{ FString::Printf(TEXT("WG_SkillSelectSlot_P_%d"), i) };
		skillSlots[START_SKILL_WIDGET::ACTIVE_SLOT_COUNT + i] = CastChecked<UDWP_SelectSkillSlot>(GetWidgetFromName(currentSlotName));
	}
}

void UDWP_StartSkill::NativeConstruct()
{
	Super::NativeConstruct();

	ownerPlayerController = Cast<ADAPC_Select>(GetOwnerPlayerController());
	DFCHECK(ownerPlayerController != nullptr);

	TArray<FDFActiveSkillData*> activeSkillData{};
	TArray<FDFPassiveSkillData*> passiveSkillData{};

	activeDataTable->GetAllRows<FDFActiveSkillData>(TEXT("GetAllRows"), activeSkillData);
	passiveDataTable->GetAllRows<FDFPassiveSkillData>(TEXT("GetAllRows"), passiveSkillData);

	//각 위젯 셋팅해 주기
	for (int32 i = 0; i < activeSkillData.Num(); ++i) {
		auto skillCDO = activeSkillData[i]->activeSkillClass->GetDefaultObject<UDO_SkillBase>();
		skillSlots[i]->SetSkillName(skillCDO->GetSkillName());
		skillSlots[i]->SetSkillImage(skillCDO->GetSkillIcon());
		skillSlots[i]->SetSkillType(true);
		skillSlots[i]->onChecked.AddDynamic(this, &UDWP_StartSkill::OnCheck);
	}

	for (int32 i = activeSkillData.Num(); i < activeSkillData.Num() + passiveSkillData.Num(); ++i) {
		auto skillCDO = passiveSkillData[i - activeSkillData.Num()]->passiveSkillClass->GetDefaultObject<UDO_SkillBase>();
		skillSlots[i]->SetSkillName(skillCDO->GetSkillName());
		skillSlots[i]->SetSkillImage(skillCDO->GetSkillIcon());
		skillSlots[i]->SetSkillType(false);
		skillSlots[i]->onChecked.AddDynamic(this, &UDWP_StartSkill::OnCheck);
	}

	btn_Check->OnClicked.AddDynamic(this, &UDWP_StartSkill::OnClickedCheck);
	btn_Reset->OnClicked.AddDynamic(this, &UDWP_StartSkill::OnClickedReset);
	btn_Back->OnClicked.AddDynamic(ownerPlayerController, &ADAPC_Select::ShowCharacterSelectWidget);
}

void UDWP_StartSkill::NativeDestruct()
{
	Super::NativeDestruct();

	for (auto skillSlot : skillSlots)
		skillSlot->onChecked.RemoveDynamic(this, &UDWP_StartSkill::OnCheck);

	btn_Check->OnClicked.RemoveDynamic(this, &UDWP_StartSkill::OnClickedCheck);
	btn_Reset->OnClicked.RemoveDynamic(this, &UDWP_StartSkill::OnClickedReset);
	btn_Back->OnClicked.RemoveDynamic(ownerPlayerController, &ADAPC_Select::ShowCharacterSelectWidget);
}

void UDWP_StartSkill::OnClickedCheck()
{
	if (ownerPlayerController->IsAllCheckedSkills()) 
		ownerPlayerController->GameStart();
	
	else
		ownerPlayerController->ShowPopupBoxWidget(FText::FromString(TEXT("Please check 3 active and 1 passive")));
}

void UDWP_StartSkill::OnClickedReset()
{
	for (auto skillSlot : skillSlots)
		skillSlot->SetCheckState(false);
	ownerPlayerController->ResetSkills();
}

void UDWP_StartSkill::OnCheck(UDWP_SelectSkillSlot* _CheckedSlot, bool _IsChecked)
{
	//체크상태에따라 분기처리를 해줌
	if (_IsChecked) {
		if (_CheckedSlot->IsActiveSkill()) {
			int32 index = ownerPlayerController->SetActiveSkill(_CheckedSlot->GetSkillName());
			if (index >= 0)
				_CheckedSlot->SetIndex(index);
			else
				_CheckedSlot->SetCheckState(false);
		}
		else {
			for (auto skillSlot : skillSlots)
				if (!skillSlot->IsActiveSkill() && skillSlot != _CheckedSlot)
					skillSlot->SetCheckState(false);
			ownerPlayerController->SetPassiveSkill(_CheckedSlot->GetSkillName());
		}
	}
	else
		if (_CheckedSlot->IsActiveSkill())
			ownerPlayerController->RemoveActiveSkill(_CheckedSlot->GetSkillName());
		else
			ownerPlayerController->RemovePassiveSkill(_CheckedSlot->GetSkillName());
}
