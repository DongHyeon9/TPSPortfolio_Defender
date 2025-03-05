#include "Widget/InGame/DWP_ReturnMenu.h"
#include "Actor/Player/Controller/DAPC_InGame.h"
#include "Widget/Select/DWP_PopupBox.h"

#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

UDWP_ReturnMenu::UDWP_ReturnMenu(const FObjectInitializer& _ObjectInitializer) :
	Super(_ObjectInitializer)
{
	robbyName = TEXT("LV_Start");
}

bool UDWP_ReturnMenu::ShowInViewport(int32 _Order)
{
	bool result = Super::ShowInViewport(_Order);
	GetOwnerPlayerController()->SetShowMouseCursor(true);
	GetOwnerPlayerController()->SetInputMode(FInputModeUIOnly());

	if(result)
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.f);

	return result;
}

bool UDWP_ReturnMenu::RemoveInViewport()
{
	bool result = Super::RemoveInViewport();
	GetOwnerPlayerController()->SetShowMouseCursor(false);
	GetOwnerPlayerController()->SetInputMode(FInputModeGameOnly());

	if (result)
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);

	return result;
}

void UDWP_ReturnMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (popupBoxWidget == nullptr)
		popupBoxWidget = CreateWidget<UDWP_PopupBox>(this, popupBoxWidgetClass);

	btn_ReturnToGame->OnClicked.AddDynamic(this, &UDWP_ReturnMenu::OnClickedToGame);
	btn_ReturnToRobby->OnClicked.AddDynamic(this, &UDWP_ReturnMenu::OnClickedToRobby);
	btn_SaveGame->OnClicked.AddDynamic(this, &UDWP_ReturnMenu::OnClickedSaveGame);
}

void UDWP_ReturnMenu::NativeDestruct()
{
	Super::NativeDestruct();

	popupBoxWidget->RemoveInViewport();

	btn_ReturnToGame->OnClicked.RemoveDynamic(this, &UDWP_ReturnMenu::OnClickedToGame);
	btn_ReturnToRobby->OnClicked.RemoveDynamic(this, &UDWP_ReturnMenu::OnClickedToRobby);
	btn_SaveGame->OnClicked.RemoveDynamic(this, &UDWP_ReturnMenu::OnClickedSaveGame);
}

void UDWP_ReturnMenu::OnClickedToRobby()
{
	//로비로 돌아갑니다
	UGameplayStatics::OpenLevel(GetOwnerPlayerController(), robbyName);
}

void UDWP_ReturnMenu::OnClickedToGame()
{
	//다시 게임으로 돌아갑니다
	GetOwnerPlayerController()->SetShowMouseCursor(false);
	GetOwnerPlayerController()->SetInputMode(FInputModeGameOnly());
	RemoveInViewport();
}

void UDWP_ReturnMenu::OnClickedSaveGame()
{
	//게임을 저장합니다
	auto playerController = Cast<ADAPC_InGame>(GetOwnerPlayerController());
	DFCHECK(playerController != nullptr);

	playerController->SaveGame() ?
		popupBoxWidget->SetDescriptionText(FText::FromString(TEXT("Save Success!!"))) :
		popupBoxWidget->SetDescriptionText(FText::FromString(TEXT("Save Fail...")));

	popupBoxWidget->ShowInViewport(VO_POPUP);
}