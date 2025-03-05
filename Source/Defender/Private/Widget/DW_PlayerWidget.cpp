#include "Widget/DW_PlayerWidget.h"
#include "Actor/Player/DA_PlayerController.h"
#include "Actor/Player/DA_Player.h"
#include "Actor/Player/DA_PlayerState.h"

UDW_PlayerWidget::UDW_PlayerWidget(const FObjectInitializer& _ObjectInitializer):
	Super(_ObjectInitializer)
{
}

void UDW_PlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ownerController = GetOwningPlayer<ADA_PlayerController>();
	DFCHECK(ownerController != nullptr);

	ownerPlayer = ownerController->GetPawn<ADA_Player>();
	DFCHECK(ownerPlayer != nullptr);

	ownerState = ownerController->GetPlayerState<ADA_PlayerState>();
	DFCHECK(ownerState != nullptr);
}

FText UDW_PlayerWidget::SetMagazineText(const FWeaponOwnedData& _WeaponOwnedData)
{
	//총알수가 음수라면 무한으로 설정해준다
	FString total = _WeaponOwnedData.total < 0 ? FString(TEXT("Infi")) : FString::FromInt(_WeaponOwnedData.total);
	
	return FText::FromString(FString(FString::FromInt(_WeaponOwnedData.current) + FString(TEXT(" | ")) + total));
}

FText UDW_PlayerWidget::SetGoldText(int32 _Gold)
{
#define LOCTEXT_NAMESPACE "DFKor"
	FText result = FText::Format(LOCTEXT("GoldFormat", "{0} g"), FText::AsNumber(_Gold));
#undef LOCTEXT_NAMESPACE
	return result;
}

FText UDW_PlayerWidget::SetTimeText(int32 _Time)
{
	int32 minute = _Time / 60;
	int32 seconds = _Time % 60;
	FTimespan time;
#define LOCTEXT_NAMESPACE "DFKor"
	FText result = FText::Format(LOCTEXT("GoldFormat", "{0} : {1}"), FText::AsNumber(minute), FText::AsNumber(seconds));
#undef LOCTEXT_NAMESPACE
	return result;
}
