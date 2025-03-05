#include "Widget/InGame/Information/DWP_Infomation.h"
#include "Widget/InGame/Information/DWP_PotionSlot.h"
#include "Widget/InGame/Information/DWP_WeaponSlot.h"
#include "Widget/InGame/Information/DWP_EquipDropButton.h"
#include "Component/Share/DC_Stat.h"
#include "Component/Player/DC_Weapon.h"
#include "Component/Player/DC_Inventory.h"
#include "Actor/Player/DA_Player.h"
#include "Actor/Player/DA_PlayerController.h"
#include "Actor/Player/DA_PlayerState.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"

UDWP_Infomation::UDWP_Infomation(const FObjectInitializer& _ObjectInitializer):
	Super(_ObjectInitializer)
{
	bIsFocusable = true;
}

void UDWP_Infomation::InitializeWidget()
{
	Super::InitializeWidget();

	//모든 위젯을 가져와서 각 배열에 담아준다
	TArray<UWidget*> allWidgets{};
	WidgetTree->GetAllWidgets(allWidgets);

	for (auto widget : allWidgets) {
		auto weaponSlot = Cast<UDWP_WeaponSlot>(widget);
		if (weaponSlot != nullptr) {
			weaponSlots.Add(weaponSlot);
			continue;
		}

		auto potionSlot = Cast<UDWP_PotionSlot>(widget);
		if (potionSlot != nullptr) {
			potionSlots.Add(potionSlot);
			continue;
		}
	}
}

void UDWP_Infomation::NativeConstruct()
{
	Super::NativeConstruct();

	//InputMode가 UIOnly여도 지울 수 있게 키입력을 받을 수 있게 해준다
	SetKeyboardFocus();

	//장착,버림 위젯을 생성한다
	equipDropButtonWidget = CreateWidget<UDWP_EquipDropButton>(GetOwnerPlayerController(), equipDropButtonWidgetClass);
	DFCHECK(equipDropButtonWidgetClass != nullptr);

	equipDropButtonWidget->ShowInViewport(VO_EQUIP_DROP);
	equipDropButtonWidget->SetVisibility(ESlateVisibility::Hidden);

	//생성한 위젯의 클릭시 호출되는 델리게이트에 연결해준다
	equipDropButtonWidget->GetEquipButton()->OnClicked.AddDynamic(this, &UDWP_Infomation::SetKeyboardFocus);
	equipDropButtonWidget->GetDropButton()->OnClicked.AddDynamic(this, &UDWP_Infomation::SetKeyboardFocus);

	//각각의 필요한 정보들을 받아오기위해 델리게이트를 연결해준다
	UDC_Weapon* weaponComp = GetOwnerPlayerCharacter()->GetWeaponComponent();
	UDC_Inventory* inventoryComp = GetOwnerPlayerCharacter()->GetInventoryComponent();
	UDC_Stat* statComp = GetOwnerPlayerCharacter()->GetStatComponent();
	
	statComp->onChangeEXP.AddDynamic(this, &UDWP_Infomation::SetEXP);
	statComp->onChangeHP.AddDynamic(this, &UDWP_Infomation::SetHP);
	statComp->onChangeLevel.AddDynamic(this, &UDWP_Infomation::SetLevel);
	statComp->onChangeSpeed.AddDynamic(this, &UDWP_Infomation::SetSpeed);
	statComp->onChangeDamage.AddDynamic(this, &UDWP_Infomation::SetDamage);
	inventoryComp->onGoldChange.AddDynamic(this, &UDWP_Infomation::SetGold);
	GetOwnerPlayerState()->onNameChange.AddDynamic(this, &UDWP_Infomation::SetPlayerName);
	GetOwnerPlayerState()->onScoreChange.AddDynamic(this, &UDWP_Infomation::SetScore);

	//위젯들의 표시되는 값을 한번 셋팅해 준다
	const auto& currentStat = statComp->GetCharacterCurrentStat();
	const auto& currentStatData = statComp->GetCurrentDataTable();

	txt_Name->SetText(FText::FromString(GetOwnerPlayerState()->GetPlayerName()));
	SetEXP(statComp->GetCurrentDataTable().nextExp, currentStat.currentExp);
	SetHP(currentStatData.characterHP, currentStat.currentHP);
	SetLevel(currentStat.characterLevel);
	SetSpeed(statComp->GetSpeed(ESTAT_DATA_TYPE::DEFAULT), statComp->GetSpeed(ESTAT_DATA_TYPE::ADDITION));
	SetDamage(statComp->GetDamage(ESTAT_DATA_TYPE::DEFAULT), statComp->GetDamage(ESTAT_DATA_TYPE::ADDITION));
	SetGold(inventoryComp->GetCurrentGold());
	SetScore(GetOwnerPlayerState()->GetCurrentScore(), GetOwnerPlayerState()->GetHighScore());

	//등록되어있는 무기위젯과 포션위젯을 가져온다
	//각각의 위젯에 필요한 데이터들을 셋팅해주고 델리게이트도 연결해준다
	
	TArray<FWeaponOwnedData> weaponOwnedData = weaponComp->GetAllWeaponData();

	const TMap<FString, int32>& itemMap = inventoryComp->GetItemMap();
	TArray<FDFItem*> itemData{};
	itemDataTable->GetAllRows<FDFItem>(TEXT("GetAllRows"), itemData);

	int32 last = weaponOwnedData.Num() > weaponSlots.Num() ? weaponSlots.Num() : weaponOwnedData.Num();

	for (int32 i = 0; i < last; ++i) {
		weaponSlots[i]->InitializeWeaponSlot(weaponOwnedData[i]);
		weaponSlots[i]->onButtonClicked.AddDynamic(this, &UDWP_Infomation::OnClicked);
	}

	last = potionSlots.Num();
	int32 usingPotionSlotCount{};
	for (int32 i = 0; i < itemData.Num()&& usingPotionSlotCount< last; ++i) {
		FString potionName = itemData[i]->itemName;
		if (!itemMap.Contains(potionName)) continue;
		potionSlots[i]->onButtonClicked.AddDynamic(this, &UDWP_Infomation::OnClicked);
		FPotionSlotInitializer init{};
		init.itemData = itemData[i];
		init.currentCount = itemMap[potionName];
		potionSlots[i]->InitializePotionSlot(init);
		++usingPotionSlotCount;
	}
}

void UDWP_Infomation::NativeDestruct()
{
	Super::NativeDestruct();

	//연결했던 델리게이트들을 지워준다
	equipDropButtonWidget->GetEquipButton()->OnClicked.RemoveDynamic(this, &UDWP_Infomation::SetKeyboardFocus);
	equipDropButtonWidget->GetDropButton()->OnClicked.RemoveDynamic(this, &UDWP_Infomation::SetKeyboardFocus);

	equipDropButtonWidget->RemoveInViewport();

	UDC_Weapon* weaponComp = GetOwnerPlayerCharacter()->GetWeaponComponent();
	UDC_Inventory* inventoryComp = GetOwnerPlayerCharacter()->GetInventoryComponent();
	UDC_Stat* statComp = GetOwnerPlayerCharacter()->GetStatComponent();

	statComp->onChangeEXP.RemoveDynamic(this, &UDWP_Infomation::SetEXP);
	statComp->onChangeHP.RemoveDynamic(this, &UDWP_Infomation::SetHP);
	statComp->onChangeLevel.RemoveDynamic(this, &UDWP_Infomation::SetLevel);
	statComp->onChangeSpeed.RemoveDynamic(this, &UDWP_Infomation::SetSpeed);
	statComp->onChangeDamage.RemoveDynamic(this, &UDWP_Infomation::SetDamage);
	inventoryComp->onGoldChange.RemoveDynamic(this, &UDWP_Infomation::SetGold);
	GetOwnerPlayerState()->onNameChange.RemoveDynamic(this, &UDWP_Infomation::SetPlayerName);
	GetOwnerPlayerState()->onScoreChange.RemoveDynamic(this, &UDWP_Infomation::SetScore);

	TArray<FDFWeaponData*> weaponData{};
	weaponDataTable->GetAllRows<FDFWeaponData>(TEXT("GetAllRows"), weaponData);

	TArray<FDFItem*> itemData{};
	itemDataTable->GetAllRows<FDFItem>(TEXT("GetAllRows"), itemData);

	int32 last = weaponData.Num() > weaponSlots.Num() ? weaponSlots.Num() : weaponData.Num();

	for (int32 i = 0; i < last; ++i)
		weaponSlots[i]->onButtonClicked.RemoveDynamic(this, &UDWP_Infomation::OnClicked);

	last = itemData.Num() > potionSlots.Num() ? potionSlots.Num() : itemData.Num();
	
	for (int32 i = 0; i < last; ++i)
		potionSlots[i]->onButtonClicked.RemoveDynamic(this, &UDWP_Infomation::OnClicked);

	GetOwnerPlayerController()->SetInputMode(FInputModeGameOnly());
	GetOwnerPlayerController()->SetShowMouseCursor(false);
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}

FReply UDWP_Infomation::NativeOnKeyDown(const FGeometry& _InGeometry, const FKeyEvent& _InKeyEvent)
{
	FReply result = Super::NativeOnKeyDown(_InGeometry, _InKeyEvent);

	//키입력을 받아 UI를 지울수 있게 한다
	if (_InKeyEvent.GetKey() != EKeys::I) return result;

	DFLOG_N(Display);
	DFCHECK(RemoveInViewport(), result);
	return result;
}

void UDWP_Infomation::OnClicked(const FString& _ItemName, EITEM_TYPE _ItemType)
{
	DFLOG_N(Display);

	equipDropButtonWidget->SetVisibility(ESlateVisibility::Visible);

	//위젯을 클릭하면 정보를 셋팅하고 클릭한 위치로 위젯을 옮긴다
	equipDropButtonWidget->SetItemName(_ItemName);
	equipDropButtonWidget->SetItemType(_ItemType);

	float xPos{}, yPos{};
	
	float viewportScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
	UWidgetLayoutLibrary::GetMousePositionScaledByDPI(GetOwningPlayer(), xPos, yPos);

	FVector2D widgetSize = equipDropButtonWidget->GetWidgetSize();

	xPos -= widgetSize.X * 0.5;
	yPos -= widgetSize.Y * 0.5;

	equipDropButtonWidget->SetPositionInViewport(FVector2D(xPos, yPos) * viewportScale);

	if (widgetSize.Size() > 1000.f)
		OnClicked(_ItemName, _ItemType);
}

void UDWP_Infomation::SetEXP(int32 _TotalEXP, int32 _CurrentEXP)
{
	txt_EXPTotal->SetText(FText::AsNumber(_TotalEXP));
	txt_EXPCurrent->SetText(FText::AsNumber(_CurrentEXP));
}

void UDWP_Infomation::SetLevel(int32 _Level)
{
	txt_Level->SetText(FText::AsNumber(_Level));
}

void UDWP_Infomation::SetGold(int32 _TotalGold)
{
#define LOCTEXT_NAMESPACE "DFKor"
	txt_GoldHas->SetText(SetGoldText(_TotalGold));
#undef LOCTEXT_NAMESPACE
}

void UDWP_Infomation::SetHP(float _MaxHP, float _CurrentHP)
{
	txt_HPMax->SetText(FText::AsNumber((int32)_MaxHP));
	txt_HPCurrent->SetText(FText::AsNumber((int32)_CurrentHP));
}

void UDWP_Infomation::SetSpeed(float _BaseSpeed, float _AdditionalSpeed)
{
#define LOCTEXT_NAMESPACE "DFKor"
	txt_SpeedBase->SetText(FText::AsNumber(_BaseSpeed));
	txt_SpeedAdd->SetText(FText::Format(LOCTEXT("AdditionalStatFormat", "+ {0}"), FText::AsNumber(_AdditionalSpeed)));
#undef LOCTEXT_NAMESPACE
}

void UDWP_Infomation::SetDamage(float _BaseDamage, float _AdditionalDamage)
{
#define LOCTEXT_NAMESPACE "DFKor"
	txt_DamageBase->SetText(FText::AsNumber(_BaseDamage));
	txt_DamageAdd->SetText(FText::Format(LOCTEXT("AdditionalStatFormat", "+ {0}"), FText::AsNumber(_AdditionalDamage)));
#undef LOCTEXT_NAMESPACE
}

void UDWP_Infomation::SetScore(int32 _Current, int32 _High)
{
	txt_ScoreCurrent->SetText(FText::AsNumber(_Current));
	txt_ScoreBest->SetText(FText::AsNumber(_High));
}

void UDWP_Infomation::SetPlayerName(const FString& _Name)
{
	txt_Name->SetText(FText::FromString(_Name));
}
