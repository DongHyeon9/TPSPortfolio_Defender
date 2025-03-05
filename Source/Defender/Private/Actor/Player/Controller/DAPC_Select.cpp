#include "Actor/Player/Controller/DAPC_Select.h"
#include "Server/DA_GameInstance.h"
#include "Widget/Select/DWP_StartCharacter.h"
#include "Widget/Select/DWP_StartLoad.h"
#include "Widget/Select/DWP_StartMain.h"
#include "Widget/Select/DWP_StartSkill.h"
#include "Widget/Select/DWP_PopupBox.h"

#include "Kismet/KismetSystemLibrary.h"

ADAPC_Select::ADAPC_Select()
{
	beginStageName = TEXT("LV_Stage1");
	activeSkills.Init(FString(), 3);
}

FString ADAPC_Select::SaveJson()
{
	Super::SaveJson();

	//플레이어 데이터가 없다면 플레이어 데이터를 새로 생성합니다
	if (playerData.IsEmpty()) {
		TSharedRef<TJsonWriter<TCHAR>> jsonWriter = TJsonWriterFactory<>::Create(&playerData);

		jsonWriter->WriteObjectStart();
		jsonWriter->WriteObjectStart(TEXT("PlayerInfo"));
		jsonWriter->WriteValue(TEXT("PlayerName"), playerName);
		jsonWriter->WriteValue(TEXT("PlayerIndex"), characterIndex);
		jsonWriter->WriteValue(TEXT("CurrentScore"), 0);
		jsonWriter->WriteValue(TEXT("HighScore"), 0);
		jsonWriter->WriteObjectEnd();

		jsonWriter->WriteObjectStart(TEXT("Stat"));
		jsonWriter->WriteValue(TEXT("Level"), 1);
		jsonWriter->WriteValue(TEXT("EXP"), 0);
		jsonWriter->WriteObjectEnd();

		jsonWriter->WriteObjectStart(TEXT("Inventory"));
		jsonWriter->WriteValue(TEXT("Gold"), 0);
		jsonWriter->WriteObjectEnd();

		jsonWriter->WriteObjectStart(TEXT("Weapon"));
		jsonWriter->WriteArrayStart(TEXT("OwnedWeapon"));
		jsonWriter->WriteObjectStart();
		jsonWriter->WriteValue(TEXT("WeaponName"), TEXT("PistolW"));
		jsonWriter->WriteValue(TEXT("OwnedState"), 2);
		jsonWriter->WriteValue(TEXT("TotalMagazine"), -1);
		jsonWriter->WriteValue(TEXT("CurrentMagazine"), 15);
		jsonWriter->WriteObjectEnd();
		jsonWriter->WriteArrayEnd();
		jsonWriter->WriteValue(TEXT("CurrentWeapon"), TEXT("PistolW"));
		jsonWriter->WriteObjectEnd();

		jsonWriter->WriteObjectStart(TEXT("Skill"));
		jsonWriter->WriteArrayStart(TEXT("ActiveSkill"));
		jsonWriter->WriteObjectStart();
		jsonWriter->WriteValue(TEXT("Key"), TEXT("One"));
		jsonWriter->WriteValue(TEXT("SkillName"), activeSkills[0]);
		jsonWriter->WriteObjectEnd();
		jsonWriter->WriteObjectStart();
		jsonWriter->WriteValue(TEXT("Key"), TEXT("Two"));
		jsonWriter->WriteValue(TEXT("SkillName"), activeSkills[1]);
		jsonWriter->WriteObjectEnd();
		jsonWriter->WriteObjectStart();
		jsonWriter->WriteValue(TEXT("Key"), TEXT("Three"));
		jsonWriter->WriteValue(TEXT("SkillName"), activeSkills[2]);
		jsonWriter->WriteObjectEnd();
		jsonWriter->WriteArrayEnd();

		jsonWriter->WriteArrayStart(TEXT("PassiveSkill"));
		jsonWriter->WriteObjectStart();
		jsonWriter->WriteValue(TEXT("Key"), passiveSkill);
		jsonWriter->WriteValue(TEXT("SkillName"), passiveSkill);
		jsonWriter->WriteObjectEnd();
		jsonWriter->WriteArrayEnd();
		jsonWriter->WriteObjectEnd();
		jsonWriter->WriteObjectEnd();
		jsonWriter->Close();
	}

	return playerData;
}

bool ADAPC_Select::LoadGame(const FString& _PlayerName)
{
	DFLOG_N(Display);

	//지정된 디렉토리에서 플레이어의 데이터를 가져옵니다
	FString loadDirectory = UKismetSystemLibrary::GetProjectSavedDirectory();
	loadDirectory += _PlayerName;
	DFLOG(Warning, TEXT("%s"), *loadDirectory);
	auto gameInstance = GetGameInstance<UDA_GameInstance>();

	if (FFileHelper::LoadFileToString(playerData, *loadDirectory)) {
		DFLOG(Warning, TEXT("Load Game Success"));
		return true;
	}
	else {
		DFLOG(Error, TEXT("Load Game Fail"));
		return false;
	}
}

void ADAPC_Select::ShowMainWidget()
{
	ChangeCurrentWidget(startMainWidget);
}

void ADAPC_Select::ShowLoadWidget()
{
	ChangeCurrentWidget(startLoadMainWidget);
}

void ADAPC_Select::ShowCharacterSelectWidget()
{
	ChangeCurrentWidget(characterSelectWidget);
}

void ADAPC_Select::ShowSkillSelectWidget()
{
	ChangeCurrentWidget(skillSelectWidget);
}

void ADAPC_Select::GameStart()
{
	popupBoxWidget->RemoveInViewport();
	OpenLevel(beginStageName);
}

void ADAPC_Select::ResetSkills()
{
	for (auto& activeSkill : activeSkills)
		activeSkill.Empty();

	passiveSkill.Empty();
}

void ADAPC_Select::ShowPopupBoxWidget(const FText& _NewText)
{
	popupBoxWidget->SetDescriptionText(_NewText);
	popupBoxWidget->ShowInViewport(VO_POPUP_S);
}

void ADAPC_Select::SetPlayerName(const FString& _PlayerName)
{
	playerName = _PlayerName;
}

void ADAPC_Select::SetCharacterIndex(int32 _Index)
{
	characterIndex = _Index;
}

int32 ADAPC_Select::SetActiveSkill(const FString& _SkillName)
{
	for (int32 i = 0; i < activeSkills.Num(); ++i)
		if (activeSkills[i].IsEmpty()) {
			activeSkills[i] = _SkillName;
			return i;
		}

	return -1;
}

void ADAPC_Select::RemoveActiveSkill(const FString& _SkillName)
{
	if (activeSkills.Contains(_SkillName))
		activeSkills[activeSkills.Find(_SkillName)].Empty();
}

void ADAPC_Select::RemovePassiveSkill(const FString& _SkillName)
{
	if (passiveSkill == _SkillName)
		passiveSkill.Empty();
}

void ADAPC_Select::SetPassiveSkill(const FString& _SkillName)
{
	passiveSkill = _SkillName;
}

bool ADAPC_Select::IsAllCheckedSkills()
{
	for (auto activeSkill : activeSkills)
		if (activeSkill.IsEmpty()) return false;

	if (passiveSkill.IsEmpty()) return false;

	return true;
}

void ADAPC_Select::BeginPlay()
{
	Super::BeginPlay();

	if (activeSkills.IsEmpty())
		activeSkills.Init(FString(), 3);

	SetInputMode(FInputModeUIOnly());
	SetShowMouseCursor(true);

	startMainWidget = CreateWidget<UDWP_StartMain>(this, startMainWidgetClass);

	startLoadMainWidget = CreateWidget<UDWP_StartLoad>(this, startLoadWidgetClass);

	characterSelectWidget = CreateWidget<UDWP_StartCharacter>(this, characterSelectWidgetClass);
	characterSelectWidget->InitializeWidget();

	skillSelectWidget = CreateWidget<UDWP_StartSkill>(this, skillSelectWidgetClass);
	skillSelectWidget->InitializeWidget();

	popupBoxWidget = CreateWidget<UDWP_PopupBox>(this, popupBoxWidgetClass);

	startMainWidget->ShowInViewport(VO_MAIN_S);
	currentShowWidget = startMainWidget;
}

void ADAPC_Select::ChangeCurrentWidget(UDW_PlayerWidget* _NewWidget)
{
	currentShowWidget->RemoveInViewport();
	popupBoxWidget->RemoveInViewport();
	currentShowWidget = _NewWidget;
	currentShowWidget->ShowInViewport(VO_MAIN_S);
}