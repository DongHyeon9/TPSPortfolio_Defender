#include "Widget/Select/DWP_StartMain.h"
#include "Actor/Player/Controller/DAPC_Select.h"

#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UDWP_StartMain::NativeConstruct()
{
	Super::NativeConstruct();

	ownerPlayerController = Cast<ADAPC_Select>(GetOwnerPlayerController());
	DFCHECK(ownerPlayerController != nullptr);

	btn_Start->OnClicked.AddDynamic(ownerPlayerController, &ADAPC_Select::ShowCharacterSelectWidget);
	btn_Load->OnClicked.AddDynamic(ownerPlayerController, &ADAPC_Select::ShowLoadWidget);
	btn_Quit->OnClicked.AddDynamic(this, &UDWP_StartMain::OnClickedQuit);
}

void UDWP_StartMain::NativeDestruct()
{
	Super::NativeDestruct();

	btn_Start->OnClicked.RemoveDynamic(ownerPlayerController, &ADAPC_Select::ShowCharacterSelectWidget);
	btn_Load->OnClicked.RemoveDynamic(ownerPlayerController, &ADAPC_Select::ShowLoadWidget);
	btn_Quit->OnClicked.RemoveDynamic(this, &UDWP_StartMain::OnClickedQuit);
}

void UDWP_StartMain::OnClickedQuit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwnerPlayerController(), EQuitPreference::Quit, true);
}
