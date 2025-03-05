#include "Widget/Select/DWP_StartLoad.h"
#include "Actor/Player/Controller/DAPC_Select.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"

void UDWP_StartLoad::NativeConstruct()
{
	Super::NativeConstruct();

	ownerPlayerController = Cast<ADAPC_Select>(GetOwnerPlayerController());
	DFCHECK(ownerPlayerController != nullptr);

	btn_Check->OnClicked.AddDynamic(this, &UDWP_StartLoad::OnClickedCheck);
	btn_Back->OnClicked.AddDynamic(ownerPlayerController, &ADAPC_Select::ShowMainWidget);
}

void UDWP_StartLoad::NativeDestruct()
{
	Super::NativeDestruct();

	btn_Check->OnClicked.RemoveDynamic(this, &UDWP_StartLoad::OnClickedCheck);
	btn_Back->OnClicked.RemoveDynamic(ownerPlayerController, &ADAPC_Select::ShowMainWidget);
}

void UDWP_StartLoad::OnClickedCheck()
{
	if (ownerPlayerController->LoadGame(etb_ID->GetText().ToString()))
		ownerPlayerController->GameStart();

	else
		ownerPlayerController->ShowPopupBoxWidget(FText::FromString(TEXT("Unidentified character name")));
}
