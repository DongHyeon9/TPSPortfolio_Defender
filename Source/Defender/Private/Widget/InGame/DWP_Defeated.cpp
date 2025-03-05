#include "Widget/InGame/DWP_Defeated.h"
#include "Actor/Player/DA_PlayerController.h"
#include "Actor/Player/DA_PlayerState.h"

#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

UDWP_Defeated::UDWP_Defeated(const FObjectInitializer& _ObjectInitializer) :
	Super(_ObjectInitializer)
{
	robbyName = TEXT("LV_Start");
}

bool UDWP_Defeated::ShowInViewport(int32 _Order)
{
	//뷰포트에 표시하고 마우스를 표시한다
	//뷰포트에 띄웠다면 게임을 정지한다
	bool result = Super::ShowInViewport(_Order);
	GetOwnerPlayerController()->SetShowMouseCursor(true);
	GetOwnerPlayerController()->SetInputMode(FInputModeUIOnly());

	if (result)
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.f);

	return result;
}

void UDWP_Defeated::NativeConstruct()
{
	Super::NativeConstruct();

	btn_ToRobby->OnClicked.AddDynamic(this, &UDWP_Defeated::OnClickedToRobby);

	auto playerState = GetOwningPlayerState<ADA_PlayerState>();
	DFCHECK(playerState != nullptr);

	txt_Score->SetText(FText::AsNumber(playerState->GetCurrentScore()));
}

void UDWP_Defeated::NativeDestruct()
{
	Super::NativeDestruct();

	btn_ToRobby->OnClicked.RemoveDynamic(this, &UDWP_Defeated::OnClickedToRobby);
}

void UDWP_Defeated::OnClickedToRobby()
{
	UGameplayStatics::OpenLevel(GetOwnerPlayerController(), robbyName);
}
