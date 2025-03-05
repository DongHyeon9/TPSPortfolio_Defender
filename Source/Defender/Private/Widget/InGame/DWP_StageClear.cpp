#include "Widget/InGame/DWP_StageClear.h"
#include "Actor/Player/DA_PlayerState.h"
#include "Actor/Player/DA_Player.h"
#include "Component/Player/DC_Inventory.h"
#include "Component/Share/DC_Stat.h"

#include "Components/TextBlock.h"

void UDWP_StageClear::NativeConstruct()
{
	Super::NativeConstruct();

	txt_CurrentScore->SetText(FText::AsNumber(GetOwnerPlayerState()->GetCurrentScore()));
	txt_TotalScore->SetText(FText::AsNumber(GetOwnerPlayerState()->GetHighScore()));
	txt_Level->SetText(FText::AsNumber(GetOwnerCharacter()->GetStatComponent()->GetCharacterCurrentStat().characterLevel));
	txt_Gold->SetText(FText::AsNumber(GetOwnerPlayerCharacter()->GetInventoryComponent()->GetCurrentGold()));
}
