#include "Widget/InGame/Information/DWP_MainWidget.h"
#include "Actor/Player/DA_Player.h"
#include "Actor/Player/DA_PlayerController.h"
#include "Actor/Player/DA_PlayerState.h"
#include "Component/Share/DC_Stat.h"
#include "Component/Player/DC_Weapon.h"
#include "Component/Player/DC_Inventory.h"
#include "Component/Share/DC_Skill.h"
#include "Skills/Active/DO_ActiveBase.h"
#include "Skills/Passive/DO_PassiveBase.h"
#include "Widget/InGame/Information/DWP_SkillSlot.h"

#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"

UDWP_MainWidget::UDWP_MainWidget(const FObjectInitializer& _ObjectInitializer):
	Super(_ObjectInitializer)
{
}

void UDWP_MainWidget::InitializeWidget()
{
	Super::InitializeWidget();

	//모든 위젯을 가져와서 스킬 슬롯만 배열에 넣어준다
	TArray<UWidget*> allWidgets{};
	WidgetTree->GetAllWidgets(allWidgets);

	for (auto widget : allWidgets) {
		auto skillSlot = Cast<UDWP_SkillSlot>(widget);
		if (skillSlot != nullptr)
			skillSlots.Add(skillSlot);
	}
}

void UDWP_MainWidget::SetHPBar(float _MaxHP, float _CurrentHP)
{
	pb_HPBar->SetPercent(_CurrentHP / _MaxHP);
}

void UDWP_MainWidget::SetEXPBar(int32 _TotalEXP, int32 _CurrentExp)
{
	pb_EXPBar->SetPercent((float)_CurrentExp / (float)_TotalEXP);
}

void UDWP_MainWidget::SetLevel(int32 _Level)
{
	txt_Level->SetText(FText::FromString(FString::FromInt(_Level)));
}

void UDWP_MainWidget::SetPlayerName(const FString& _NewName)
{
	txt_Name->SetText(FText::FromString(_NewName));
}

void UDWP_MainWidget::SetLevelTimer(float _NewTime)
{
	txt_Timer->SetText(SetTimerText(_NewTime));
}

void UDWP_MainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto statComp = GetOwnerPlayerCharacter()->GetStatComponent();
	auto invetoryComp = GetOwnerPlayerCharacter()->GetInventoryComponent();
	auto weaponComp = GetOwnerPlayerCharacter()->GetWeaponComponent();
	auto skillComp = GetOwnerPlayerCharacter()->GetSkillComponent();

	statComp->onChangeHP.RemoveDynamic(this, &UDWP_MainWidget::SetHPBar);
	statComp->onChangeEXP.RemoveDynamic(this, &UDWP_MainWidget::SetEXPBar);
	statComp->onChangeLevel.RemoveDynamic(this, &UDWP_MainWidget::SetLevel);
	invetoryComp->onRegistItem.RemoveDynamic(this, &UDWP_MainWidget::RegistPotion);
	invetoryComp->onItemCountChange.RemoveDynamic(this, &UDWP_MainWidget::SetPotionCount);
	weaponComp->onMagazineChange.RemoveDynamic(this, &UDWP_MainWidget::SetMaganize);
	weaponComp->onWeaponChange.RemoveDynamic(this, &UDWP_MainWidget::SetCurrentWeapon);
	weaponComp->onFireSelectorChange.RemoveDynamic(this, &UDWP_MainWidget::SetFireSelector);
	GetOwnerPlayerState()->onScoreChange.RemoveDynamic(this, &UDWP_MainWidget::SetScore);
	GetOwnerPlayerState()->onNameChange.RemoveDynamic(this, &UDWP_MainWidget::SetPlayerName);
	GetOwnerPlayerCharacter()->onCharacterChange.RemoveDynamic(this, &UDWP_MainWidget::SetCharacterImage);

	//메인위젯에 표시되는데 필요한 델리게이트들을 연결해준다
	statComp->onChangeHP.AddDynamic(this, &UDWP_MainWidget::SetHPBar);
	statComp->onChangeEXP.AddDynamic(this, &UDWP_MainWidget::SetEXPBar);
	statComp->onChangeLevel.AddDynamic(this, &UDWP_MainWidget::SetLevel);
	invetoryComp->onRegistItem.AddDynamic(this, &UDWP_MainWidget::RegistPotion);
	invetoryComp->onItemCountChange.AddDynamic(this, &UDWP_MainWidget::SetPotionCount);
	weaponComp->onMagazineChange.AddDynamic(this, &UDWP_MainWidget::SetMaganize);
	weaponComp->onWeaponChange.AddDynamic(this, &UDWP_MainWidget::SetCurrentWeapon);
	weaponComp->onFireSelectorChange.AddDynamic(this, &UDWP_MainWidget::SetFireSelector);
	GetOwnerPlayerState()->onScoreChange.AddDynamic(this, &UDWP_MainWidget::SetScore);
	GetOwnerPlayerState()->onNameChange.AddDynamic(this, &UDWP_MainWidget::SetPlayerName);
	GetOwnerPlayerCharacter()->onCharacterChange.AddDynamic(this, &UDWP_MainWidget::SetCharacterImage);

	//스킬 슬롯을 설정해준다
	WG_SkillSlot_1->SetSkillKey(TEXT("One"));
	WG_SkillSlot_1->SetSkillKeyText(TEXT("1"));
	WG_SkillSlot_2->SetSkillKey(TEXT("Two"));
	WG_SkillSlot_2->SetSkillKeyText(TEXT("2"));
	WG_SkillSlot_3->SetSkillKey(TEXT("Three"));
	WG_SkillSlot_3->SetSkillKeyText(TEXT("3"));

	for (auto skillSlot : skillSlots) {
		skillComp->onActiveChange.RemoveDynamic(skillSlot, &UDWP_SkillSlot::SetActiveSkillChange);
		skillComp->onActiveChange.AddDynamic(skillSlot, &UDWP_SkillSlot::SetActiveSkillChange);
		auto skill = skillComp->GetActiveSkill(skillSlot->GetSkillKey());
		if (skill != nullptr)
			skillSlot->SetActiveSkillChange(skillSlot->GetSkillKey(), skill, true);
	}

	skillComp->onPassiveChange.RemoveDynamic(this, &UDWP_MainWidget::SetPassiveIcon);
	skillComp->onPassiveChange.AddDynamic(this, &UDWP_MainWidget::SetPassiveIcon);

	auto passiveMap = skillComp->GetPassiveSkillMap();
	if (!passiveMap.IsEmpty())
		img_Passive->SetBrushFromTexture(skillComp->GetPassiveSkillMap().begin().Value()->GetSkillIcon());

	//현재 들고있는 무기의 이름을 등록한다
	weaponName = weaponComp->GetCurrentWeaponData().weaponData.weaponName;

	//위젯의 생성이 플레이어 캐릭터의 BeginPlay보다 느리기때문에 표시되는 위젯들을 한번 셋팅해준다
	const auto& currentStat = statComp->GetCharacterCurrentStat();
	const auto& currentStatData = statComp->GetCurrentDataTable();

	txt_Name->SetText(FText::FromString(GetOwnerPlayerState()->GetPlayerName()));
	txt_Magazine->SetText(SetMagazineText(weaponComp->GetCurrentWeaponData()));
	SetHPBar(currentStatData.characterHP, currentStat.currentHP);
	SetEXPBar(statComp->GetCurrentDataTable().nextExp, currentStat.currentExp);
	SetLevel(currentStat.characterLevel);
	SetScore(GetOwnerPlayerState()->GetCurrentScore(), GetOwnerPlayerState()->GetHighScore());
	SetCharacterImage(GetOwnerPlayerCharacter()->GetPlayerData()->characterIcon.LoadSynchronous());
	SetFireSelector(weaponComp->GetCurrentWeaponData().fireMode);
}

void UDWP_MainWidget::NativeDestruct()
{
	Super::NativeDestruct();

	auto statComp = GetOwnerPlayerCharacter()->GetStatComponent();
	auto invetoryComp = GetOwnerPlayerCharacter()->GetInventoryComponent();
	auto weaponComp = GetOwnerPlayerCharacter()->GetWeaponComponent();
	auto skillComp = GetOwnerPlayerCharacter()->GetSkillComponent();

	//연결되었던 델리게이트들을 삭제해준다
	statComp->onChangeHP.RemoveDynamic(this, &UDWP_MainWidget::SetHPBar);
	statComp->onChangeEXP.RemoveDynamic(this, &UDWP_MainWidget::SetEXPBar);
	statComp->onChangeLevel.RemoveDynamic(this, &UDWP_MainWidget::SetLevel);
	invetoryComp->onRegistItem.RemoveDynamic(this, &UDWP_MainWidget::RegistPotion);
	invetoryComp->onItemCountChange.RemoveDynamic(this, &UDWP_MainWidget::SetPotionCount);
	weaponComp->onMagazineChange.RemoveDynamic(this, &UDWP_MainWidget::SetMaganize);
	weaponComp->onWeaponChange.RemoveDynamic(this, &UDWP_MainWidget::SetCurrentWeapon);
	GetOwnerPlayerState()->onNameChange.RemoveDynamic(this, &UDWP_MainWidget::SetPlayerName);
	GetOwnerPlayerState()->onScoreChange.RemoveDynamic(this, &UDWP_MainWidget::SetScore);
	GetOwnerPlayerCharacter()->onCharacterChange.RemoveDynamic(this, &UDWP_MainWidget::SetCharacterImage);

	for (auto skillSlot : skillSlots)
		skillComp->onActiveChange.RemoveDynamic(skillSlot, &UDWP_SkillSlot::SetActiveSkillChange);
	skillComp->onPassiveChange.RemoveDynamic(this, &UDWP_MainWidget::SetPassiveIcon);
}

void UDWP_MainWidget::SetMaganize(const FWeaponOwnedData& _WeaponData)
{
	if (weaponName == _WeaponData.weaponData.weaponName)
		txt_Magazine->SetText(SetMagazineText(_WeaponData));
}

void UDWP_MainWidget::RegistPotion(const FString& _ItemName, UTexture2D* _ItemImage, int32 _ItemCount)
{
	potionName = _ItemName;
	img_Potion->SetBrushFromTexture(_ItemImage);
	txt_potionCount->SetText(FText::AsNumber(_ItemCount));
}

void UDWP_MainWidget::SetPotionCount(const FString& _ItemName, int32 _TotalItemCount)
{
	if (_ItemName != potionName)return;
	txt_potionCount->SetText(FText::AsNumber(_TotalItemCount));
}

void UDWP_MainWidget::SetCurrentWeapon(const FString& _WeaponName, EWEAPON_OWNED_STATE _WeaponOwnedState)
{
	if (_WeaponOwnedState == EWEAPON_OWNED_STATE::EQUIP)
		weaponName = _WeaponName;
}

void UDWP_MainWidget::SetScore(int32 _Current, int32 _High)
{
#define LOCTEXT_NAMESPACE "DFKor"
	txt_HighScore->SetText(FText::Format(LOCTEXT("AdditionalStatFormat", "{0}"), FText::AsNumber(_High)));
	txt_CurrentScore->SetText(FText::Format(LOCTEXT("AdditionalStatFormat", "{0}"), FText::AsNumber(_Current)));
#undef LOCTEXT_NAMESPACE
}

void UDWP_MainWidget::SetCharacterImage(UTexture2D* _CharacterIcon)
{
	img_CharacterImage->SetBrushFromTexture(_CharacterIcon);
}

FText UDWP_MainWidget::SetTimerText(float _Time)
{
	int32 min = _Time / 60.f;
	int32 sec = (int32)_Time % 60;
	FString minString{};
	FString secString{};
	if (min < 10)
		minString = FString::Printf(TEXT("0%d : "), min);
	else
		minString = FString::Printf(TEXT("%d : "), min);

	if (sec < 10)
		secString = FString::Printf(TEXT("0%d"), sec);
	else
		secString = FString::Printf(TEXT("%d"), sec);

	return 	FText::FromString(minString + secString);
}

void UDWP_MainWidget::SetPassiveIcon(const FString& _Key, UDO_PassiveBase* _Skill, bool bIsRegist)
{
	if (bIsRegist)
		img_Passive->SetBrushFromTexture(_Skill->GetSkillIcon());
}

void UDWP_MainWidget::SetFireSelector(EWEAPON_FIRE_SELECTOR _CurrentMode)
{
	UTexture2D* fireSelectorImage{};
	switch (_CurrentMode)
	{
	case EWEAPON_FIRE_SELECTOR::SEMI:
		fireSelectorImage = LoadObject<UTexture2D>(NULL, TEXT("/Game/03_Texture/UI/Main/T_Semi"));
		break;
	case EWEAPON_FIRE_SELECTOR::BURST:
		fireSelectorImage = LoadObject<UTexture2D>(NULL, TEXT("/Game/03_Texture/UI/Main/T_Burst"));
		break;
	case EWEAPON_FIRE_SELECTOR::AUTO:
		fireSelectorImage = LoadObject<UTexture2D>(NULL, TEXT("/Game/03_Texture/UI/Main/T_Auto"));
		break;
	}
	img_FireSelector->SetBrushFromTexture(fireSelectorImage);
}
