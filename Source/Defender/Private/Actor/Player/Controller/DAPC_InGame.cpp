#include "Actor/Player/Controller/DAPC_InGame.h"
#include "Actor/Player/DA_Player.h"
#include "Widget/InGame/Information/DWP_MainWidget.h"
#include "Widget/InGame/Information/DWP_Infomation.h"
#include "Server/DA_GameInstance.h"
#include "Component/Share/DC_Stat.h"
#include "Component/Player/DC_Inventory.h"
#include "Component/Player/DC_Weapon.h"
#include "Item/DA_Item.h"
#include "Component/Player/DC_Action.h"
#include "Widget/InGame/Shop/DWP_Shop.h"
#include "Component/Share/DC_Skill.h"
#include "Skills/Active/DO_ActiveBase.h"
#include "Skills/Passive/DO_PassiveBase.h"
#include "Actor/Player/DA_PlayerState.h"
#include "Widget/InGame/DWP_StageClear.h"
#include "Widget/InGame/DWP_StageStart.h"
#include "Actor/Level/DA_LevelScript.h"
#include "Widget/InGame/DWP_ReturnMenu.h"
#include "Widget/InGame/DWP_Defeated.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

ADAPC_InGame::ADAPC_InGame()
{
}

void ADAPC_InGame::CreateInGameWidgets()
{
	DFLOG_N(Display);
	//각 위젯들을 생성해 놓습니다
	mainWidget = CreateWidget<UDWP_MainWidget>(this, mainWidgetClass);
	mainWidget->InitializeWidget();

	DFCHECK(mainWidget->ShowInViewport(VO_MAIN_WIDGET));

	stageClearWidget = CreateWidget<UDWP_StageClear>(this, stageClearWidgetClass);
	auto character = Cast<ADA_Player>(GetPawn());
	DFCHECK(character != nullptr);
	stageClearWidget->SetOwnerCharacter(character);

	interactionWidget = CreateWidget<UDW_PlayerWidget>(this, interactionWidgetClass);
	returnMenuWidget = CreateWidget<UDWP_ReturnMenu>(this, returnMenuWidgetClass);
	defeatedWidget = CreateWidget<UDWP_Defeated>(this, defeatedWidgetClass);

	informationWidget = CreateWidget<UDWP_Infomation>(this, informationWidgetClass);
	informationWidget->InitializeWidget();

	stageStartWidget = CreateWidget<UDWP_StageStart>(this, stageStartWidgetClass);
	stageStartWidget->InitializeWidget();

	shopWidget = CreateWidget<UDWP_Shop>(this, shopWidgetClass);
	shopWidget->InitializeWidget();
	shopWidget->onClose.AddDynamic(this, &ADAPC_InGame::ShopClosed);
}

void ADAPC_InGame::RemoveMainWidget()
{
	DFLOG_N(Display);
	DFCHECK(mainWidget != nullptr &&
		mainWidget->RemoveInViewport());
}

void ADAPC_InGame::ShowMainWidget()
{
	DFLOG_N(Display);

	DFCHECK(mainWidget != nullptr &&
		mainWidget->ShowInViewport(VO_MAIN_WIDGET));

	SetInputMode(FInputModeGameOnly());
	SetShowMouseCursor(false);
}

UDWP_Shop* ADAPC_InGame::ShowShopWidget(float _TimeLimit)
{
	DFLOG_N(Display);

	//월드에 시간을 중지합니다
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.f);

	//다른 위젯들을 제거합니다
	interactionWidget->RemoveInViewport();
	informationWidget->RemoveInViewport();
	RemoveMainWidget();
	
	DFCHECK(shopWidget != nullptr, nullptr);

	//상점 위젯을 보여줍니다.
	//제한시간이 있다면 제한시간뒤에 상점이 자동으로 닫히게 됩니다
	shopWidget->ShowInViewport(VO_SHOP);
	FInputModeUIOnly inputMode{};
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
	inputMode.SetWidgetToFocus(shopWidget->TakeWidget());
	SetInputMode(inputMode);
	SetShowMouseCursor(true);

	shopWidget->SetTimer(_TimeLimit);

	if (_TimeLimit > 0.f) {

		auto gameInstance = GetGameInstance<UDA_GameInstance>();
		DFCHECK(gameInstance != nullptr, shopWidget);
		gameInstance->RegistTimerHandle(&shopTimerHandle);

		GetWorld()->GetTimerManager().SetTimer(shopTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				shopWidget->OnClickedClose();
				if (shopTimerHandle.IsValid()) {
					GetWorld()->GetTimerManager().ClearTimer(shopTimerHandle);
					shopTimerHandle.Invalidate();
				}
			}), _TimeLimit, false);
	}

	return shopWidget;
}

void ADAPC_InGame::ShowInteractionWidget()
{
	DFLOG_N(Display);

	DFCHECK(interactionWidget != nullptr);
	++interactionOverlappedCount;
	interactionWidget->ShowInViewport(VO_INTERACTION);
}

void ADAPC_InGame::RemoveInteractionWidget()
{
	DFLOG_N(Display);

	DFCHECK(interactionWidget != nullptr);
	--interactionOverlappedCount;
	if (interactionOverlappedCount <= 0) {
		interactionWidget->RemoveInViewport();
		interactionOverlappedCount = 0;
	}
}

void ADAPC_InGame::ShowStageStartWidget()
{
	DFLOG_N(Display);

	DFCHECK(stageStartWidget != nullptr &&
		stageStartWidget->ShowInViewport(VO_STAGE_START));
}

void ADAPC_InGame::ShowStageClearWidget()
{
	DFLOG_N(Display);

	DFCHECK(stageClearWidget != nullptr &&
		stageClearWidget->ShowInViewport(VO_STAGE_CLEAR));
}

FString ADAPC_InGame::SaveJson()
{
	FString jsonStr = Super::SaveJson();

	//플레이어의 폰과 스테이트를 가져옵니다
	auto playerCharacter = Cast<ADA_Player>(GetPawn());
	DFCHECK(playerCharacter != nullptr, FString());

	ADA_PlayerState* playerState = GetPlayerState<ADA_PlayerState>();
	DFCHECK(playerState != nullptr, FString());

	//저장할 데이터들을 관리하고있는 컴포넌트들을 가져옵니다
	UDC_Inventory* inventory = playerCharacter->GetInventoryComponent();
	UDC_Weapon* weapon = playerCharacter->GetWeaponComponent();
	UDC_Skill* skill = playerCharacter->GetSkillComponent();
	UDC_Stat* stat = playerCharacter->GetStatComponent();

	const TMap<FString, UDO_ActiveBase*> activeMap = skill->GetActiveSkillMap();
	const TMap<FString, UDO_PassiveBase*> passiveMap = skill->GetPassiveSkillMap();

	TMap<FString, int32> itemMap = inventory->GetItemMap();
	TArray<FString> itemKey{};
	TArray<int32> itemValue{};
	itemMap.GenerateKeyArray(itemKey);
	itemMap.GenerateValueArray(itemValue);

	TArray<FWeaponOwnedData> weaponData = weapon->GetAllWeaponData();

	FCharacterStat playerStat = stat->GetCharacterCurrentStat();

	TSharedRef<TJsonWriter<TCHAR>> jsonWriter = TJsonWriterFactory<>::Create(&jsonStr);

	//데이터들을 형식에 맞게 저장합니다
	jsonWriter->WriteObjectStart();
	jsonWriter->WriteObjectStart(TEXT("PlayerInfo"));
	jsonWriter->WriteValue(TEXT("PlayerName"), playerState->GetPlayerName());
	jsonWriter->WriteValue(TEXT("PlayerIndex"), playerCharacter->GetPlayerCharacterIndex());
	jsonWriter->WriteValue(TEXT("CurrentScore"), playerState->GetCurrentScore());
	jsonWriter->WriteValue(TEXT("HighScore"), playerState->GetHighScore());
	jsonWriter->WriteObjectEnd();

	jsonWriter->WriteObjectStart(TEXT("Stat"));
	jsonWriter->WriteValue(TEXT("Level"), playerStat.characterLevel);
	jsonWriter->WriteValue(TEXT("EXP"), playerStat.currentExp);
	jsonWriter->WriteObjectEnd();

	jsonWriter->WriteObjectStart(TEXT("Inventory"));
	jsonWriter->WriteArrayStart(TEXT("OwnedItem"));
	for (int32 i = 0; i < itemKey.Num(); ++i) {
		jsonWriter->WriteObjectStart();
		jsonWriter->WriteValue(TEXT("ItemName"), itemKey[i]);
		jsonWriter->WriteValue(TEXT("ItemCount"), itemValue[i]);
		jsonWriter->WriteObjectEnd();
	}
	jsonWriter->WriteArrayEnd();
	if (inventory->GetCurrentRegistedItem() != nullptr)
		jsonWriter->WriteValue(TEXT("CurrentRegistItem"), inventory->GetCurrentRegistedItem()->GetItemName());
	jsonWriter->WriteValue(TEXT("Gold"), inventory->GetCurrentGold());
	jsonWriter->WriteObjectEnd();

	jsonWriter->WriteObjectStart(TEXT("Weapon"));
	jsonWriter->WriteArrayStart(TEXT("OwnedWeapon"));
	for (int32 i = 0; i < weaponData.Num(); ++i) {
		jsonWriter->WriteObjectStart();
		jsonWriter->WriteValue(TEXT("WeaponName"), weaponData[i].weaponData.weaponName);
		jsonWriter->WriteValue(TEXT("OwnedState"), (int32)weaponData[i].ownedState);
		jsonWriter->WriteValue(TEXT("TotalMagazine"), weaponData[i].total);
		jsonWriter->WriteValue(TEXT("CurrentMagazine"), weaponData[i].current);
		jsonWriter->WriteObjectEnd();
	}
	jsonWriter->WriteArrayEnd();
	jsonWriter->WriteValue(TEXT("CurrentWeapon"), weapon->GetCurrentWeaponData().weaponData.weaponName);
	jsonWriter->WriteObjectEnd();

	jsonWriter->WriteObjectStart(TEXT("Skill"));
	jsonWriter->WriteArrayStart(TEXT("ActiveSkill"));
	for (auto active : activeMap) {
		jsonWriter->WriteObjectStart();
		jsonWriter->WriteValue(TEXT("Key"), active.Key);
		jsonWriter->WriteValue(TEXT("SkillName"), active.Value->GetSkillName());
		jsonWriter->WriteObjectEnd();
	}
	jsonWriter->WriteArrayEnd();

	jsonWriter->WriteArrayStart(TEXT("PassiveSkill"));
	for (auto passive : passiveMap) {
		jsonWriter->WriteObjectStart();
		jsonWriter->WriteValue(TEXT("Key"), passive.Key);
		jsonWriter->WriteValue(TEXT("SkillName"), passive.Value->GetSkillName());
		jsonWriter->WriteObjectEnd();
	}
	jsonWriter->WriteArrayEnd();
	jsonWriter->WriteObjectEnd();
	jsonWriter->WriteObjectEnd();
	jsonWriter->Close();

	return jsonStr;
}

bool ADAPC_InGame::LoadJson(const FString& _JsonStr)
{
	DFLOG_N(Display);

	//게임인스턴스에 있는 Json데이터를 가져옵니다
	auto gameInstance = GetGameInstance<UDA_GameInstance>();
	DFCHECK(gameInstance != nullptr, false);
	gameInstance->SaveCurrentLevelData(_JsonStr);
	DFLOG(Warning, TEXT("Load Json Success!!"));

	auto playerCharacter = Cast<ADA_Player>(GetPawn());
	DFCHECK(playerCharacter != nullptr, false);

	ADA_PlayerState* playerState = GetPlayerState<ADA_PlayerState>();
	DFCHECK(playerState != nullptr, false);

	//데이터를 로드할 컴포넌트들을 가져옵니다
	UDC_Inventory* inventory = playerCharacter->GetInventoryComponent();
	UDC_Weapon* weapon = playerCharacter->GetWeaponComponent();
	UDC_Skill* skill = playerCharacter->GetSkillComponent();
	UDC_Stat* stat = playerCharacter->GetStatComponent();

	//기본값을 생성해줍니다
	stat->SetLevel(1);
	weapon->SetPlayerController(this);
	weapon->NewStartSetting();

	TSharedRef<TJsonReader<TCHAR>> jsonReader = TJsonReaderFactory<TCHAR>::Create(_JsonStr);
	TSharedPtr<FJsonObject> json = MakeShareable(new FJsonObject());

	//json파일들의 형식에 맞춰서 데이터를 로드합니다
	if (FJsonSerializer::Deserialize(jsonReader, json) && json.IsValid()) {
		const TArray<TSharedPtr<FJsonValue>>* jsonArray{};
		const TSharedPtr<FJsonObject>* jsonObj{};
		FString outString{};
		int32 outInteger{};
		double outDouble{};
		if (json->TryGetObjectField(TEXT("PlayerInfo"), jsonObj)) {
			jsonObj->Get()->TryGetStringField(TEXT("PlayerName"), outString);
			playerState->SetPlayerName(outString);
			jsonObj->Get()->TryGetNumberField(TEXT("PlayerIndex"), outInteger);
			playerCharacter->SetPlayerCharacter(outInteger);
			jsonObj->Get()->TryGetNumberField(TEXT("CurrentScore"), outInteger);
			playerState->SetCurrentScore(outInteger);
			jsonObj->Get()->TryGetNumberField(TEXT("HighScore"), outInteger);
			playerState->SetHighScore(outInteger);
		}

		if (json->TryGetObjectField(TEXT("Stat"), jsonObj)) {
			if (jsonObj->Get()->TryGetNumberField(TEXT("Level"), outInteger))
				playerCharacter->SetStartLevel(outInteger);
			if (jsonObj->Get()->TryGetNumberField(TEXT("EXP"), outInteger))
				stat->SetEXP(outInteger);
		}

		if (json->TryGetObjectField(TEXT("Inventory"), jsonObj)) {
			if (jsonObj->Get()->TryGetArrayField(TEXT("OwnedItem"), jsonArray))
				for (auto field : *jsonArray) {
					auto obj = field->AsObject();
					if (obj->TryGetStringField(TEXT("ItemName"), outString) &&
						obj->TryGetNumberField(TEXT("ItemCount"), outInteger))
						inventory->SetItem(outString, outInteger);
				}
			if (jsonObj->Get()->TryGetStringField(TEXT("CurrentRegistItem"), outString))
				inventory->RegistItem(outString);
			if (jsonObj->Get()->TryGetNumberField(TEXT("Gold"), outInteger))
				inventory->SetGold(outInteger);
		}

		if (json->TryGetObjectField(TEXT("Weapon"), jsonObj)) {
			if (jsonObj->Get()->TryGetArrayField(TEXT("OwnedWeapon"), jsonArray)) {
				FWeaponOwnedData ownedData{};
				for (auto field : *jsonArray) {
					auto obj = field->AsObject();
					obj->TryGetStringField(TEXT("WeaponName"), outString);
					if (obj->TryGetNumberField(TEXT("OwnedState"), outInteger))
						ownedData.ownedState = (EWEAPON_OWNED_STATE)outInteger;
					obj->TryGetNumberField(TEXT("TotalMagazine"), ownedData.total);
					obj->TryGetNumberField(TEXT("CurrentMagazine"), ownedData.current);
					weapon->SetOwnedState(outString, ownedData);
				}
			}
			if (jsonObj->Get()->TryGetStringField(TEXT("CurrentWeapon"), outString))
				weapon->SetWeapon(outString);

			if (json->TryGetObjectField(TEXT("Skill"), jsonObj)) {
				FString outSkillName{};
				if (jsonObj->Get()->TryGetArrayField(TEXT("ActiveSkill"), jsonArray)) {
					for (auto field : *jsonArray) {
						auto obj = field->AsObject();
						obj->TryGetStringField(TEXT("Key"), outString);
						obj->TryGetStringField(TEXT("SkillName"), outSkillName);
						skill->AddActiveSkillbyName(outString, outSkillName);
					}
				}
				if (jsonObj->Get()->TryGetArrayField(TEXT("PassiveSkill"), jsonArray)) {
					for (auto field : *jsonArray) {
						auto obj = field->AsObject();
						obj->TryGetStringField(TEXT("Key"), outString);
						obj->TryGetStringField(TEXT("SkillName"), outSkillName);
						skill->AddPassiveSkillbyName(outString, outSkillName);
					}
				}
			}
		}
		DFLOG(Warning, TEXT("Character Setting Success"));
	}
	else {
		DFLOG(Warning, TEXT("Character Setting Fail"));
		return false;
	}
	return true;
}

void ADAPC_InGame::SetPrevLevelData()
{
	//이전 레벨의 데이터를 가져옵니다
	DFLOG_N(Display);
	auto gameInstance = GetGameInstance<UDA_GameInstance>();
	DFCHECK(gameInstance != nullptr);

	if (!LoadJson(gameInstance->GetPrevLevelData())) {
		auto playerCharacter = Cast<ADA_Player>(GetPawn());
		DFCHECK(playerCharacter != nullptr);
		UDC_Weapon* weapon = playerCharacter->GetWeaponComponent();
		playerCharacter->SetPlayerCharacter(playerCharacter->GetPlayerCharacterIndex());
		weapon->NewStartSetting();
	}
}

bool ADAPC_InGame::SaveGame()
{
	DFLOG_N(Display);
	FString jsonStr = SaveJson();

	//플레이어의 데이터를 json의 형태로 저장해서 지정된 디렉토리에 저장합니다

	ADA_PlayerState* playerState = GetPlayerState<ADA_PlayerState>();
	DFCHECK(playerState != nullptr, false);

	FString saveDirectory = UKismetSystemLibrary::GetProjectSavedDirectory();
	saveDirectory += playerState->GetPlayerName();
	DFLOG(Warning, TEXT("%s"), *saveDirectory);

	if (FFileHelper::SaveStringToFile(*jsonStr, *saveDirectory)) {
		DFLOG(Warning, TEXT("Save Success"));
		return true;
	}
	else {
		DFLOG(Error, TEXT("Save Fail"));
		return false;
	}
}

void ADAPC_InGame::ShowDefeatedWidget()
{
	DFLOG_N(Display);

	SetInputMode(FInputModeUIOnly());
	DFCHECK(defeatedWidget != nullptr &&
		defeatedWidget->ShowInViewport(VO_STAGE_DEFEATED));
}

void ADAPC_InGame::BeginPlay()
{
	Super::BeginPlay();

	auto playerCharacter = Cast<ADA_Player>(GetPawn());
	if (playerCharacter != nullptr &&
		loadingWidget->ShowInViewport(VO_LOADING_SCREEN)) {
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.f);
		playerCharacter->onAssetLoadCompleted.AddDynamic(this, &ADAPC_InGame::CloseLoadingScreen);
		SetInputMode(FInputModeUIOnly());
	}
	else
		SetInputMode(FInputModeGameOnly());
}

void ADAPC_InGame::SetupInputComponent()
{
	Super::SetupInputComponent();

	//C++에서 Input Setting 잡기
	PlayerInput->AddAxisMapping(FInputAxisKeyMapping(TEXT("MoveForward"), EKeys::W, 1.f));
	PlayerInput->AddAxisMapping(FInputAxisKeyMapping(TEXT("MoveForward"), EKeys::S, -1.f));
	PlayerInput->AddAxisMapping(FInputAxisKeyMapping(TEXT("MoveRight"), EKeys::D, 1.f));
	PlayerInput->AddAxisMapping(FInputAxisKeyMapping(TEXT("MoveRight"), EKeys::A, -1.f));
	PlayerInput->AddAxisMapping(FInputAxisKeyMapping(TEXT("Look"), EKeys::MouseY, 1.f));
	PlayerInput->AddAxisMapping(FInputAxisKeyMapping(TEXT("Turn"), EKeys::MouseX, 1.f));
	PlayerInput->AddAxisMapping(FInputAxisKeyMapping(TEXT("BaseAttack"), EKeys::LeftMouseButton, 1.f));
	PlayerInput->AddAxisMapping(FInputAxisKeyMapping(TEXT("SensitivityUp"), EKeys::RightBracket, 1.f));
	PlayerInput->AddAxisMapping(FInputAxisKeyMapping(TEXT("SensitivityDown"), EKeys::LeftBracket, 1.f));

	PlayerInput->AddActionMapping(FInputActionKeyMapping(TEXT("Jump"), EKeys::SpaceBar));
	PlayerInput->AddActionMapping(FInputActionKeyMapping(TEXT("Dash"), EKeys::LeftShift));
	PlayerInput->AddActionMapping(FInputActionKeyMapping(TEXT("ChangeCameraMode"), EKeys::V));
	PlayerInput->AddActionMapping(FInputActionKeyMapping(TEXT("Crouch"), EKeys::LeftControl));
	PlayerInput->AddActionMapping(FInputActionKeyMapping(TEXT("Zoom"), EKeys::RightMouseButton));
	PlayerInput->AddActionMapping(FInputActionKeyMapping(TEXT("ModeChange"), EKeys::B));
	PlayerInput->AddActionMapping(FInputActionKeyMapping(TEXT("Roll"), EKeys::LeftAlt));
	PlayerInput->AddActionMapping(FInputActionKeyMapping(TEXT("LockOn"), EKeys::Tab));
	PlayerInput->AddActionMapping(FInputActionKeyMapping(TEXT("TargetMoveRight"), EKeys::E));
	PlayerInput->AddActionMapping(FInputActionKeyMapping(TEXT("TargetMoveLeft"), EKeys::Q));
	PlayerInput->AddActionMapping(FInputActionKeyMapping(TEXT("MouseScrollUp"), EKeys::MouseScrollUp));
	PlayerInput->AddActionMapping(FInputActionKeyMapping(TEXT("MouseScrollDown"), EKeys::MouseScrollDown));
	PlayerInput->AddActionMapping(FInputActionKeyMapping(TEXT("Potion"), EKeys::Four));
	PlayerInput->AddActionMapping(FInputActionKeyMapping(TEXT("Reload"), EKeys::R));
	PlayerInput->AddActionMapping(FInputActionKeyMapping(TEXT("Information"), EKeys::I));
	PlayerInput->AddActionMapping(FInputActionKeyMapping(TEXT("Skill"), EKeys::One));
	PlayerInput->AddActionMapping(FInputActionKeyMapping(TEXT("Skill"), EKeys::Two));
	PlayerInput->AddActionMapping(FInputActionKeyMapping(TEXT("Skill"), EKeys::Three));
	PlayerInput->AddActionMapping(FInputActionKeyMapping(TEXT("Interaction"), EKeys::F));
	PlayerInput->AddActionMapping(FInputActionKeyMapping(TEXT("ReturnMenu"), EKeys::M));

	InputComponent->BindAction(TEXT("Information"), IE_Pressed, this, &ADAPC_InGame::ShowInformationWidget);
	InputComponent->BindAction(TEXT("ReturnMenu"), IE_Pressed, this, &ADAPC_InGame::ShowReturnMenu);
}

void ADAPC_InGame::ShowInformationWidget()
{
	DFLOG_N(Display);
	auto player = Cast<ADA_Player>(GetPawn());
	DFCHECK(player != nullptr);
	if (player->GetActionComponent()->IsZooming())
		player->GetWeaponComponent()->ZoomOut();

	DFCHECK(informationWidget->ShowInViewport(VO_INFOMATION));

	FInputModeUIOnly inputMode{};
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
	inputMode.SetWidgetToFocus(informationWidget->TakeWidget());
	SetInputMode(inputMode);
	SetShowMouseCursor(true);

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.f);
}

void ADAPC_InGame::ShowReturnMenu()
{
	DFLOG_N(Display);

	DFCHECK(returnMenuWidget != nullptr &&
		returnMenuWidget->ShowInViewport(VO_RETURN_MENU));
}

void ADAPC_InGame::ShopClosed()
{
	if (shopTimerHandle.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(shopTimerHandle);
		shopTimerHandle.Invalidate();
	}

	if (interactionOverlappedCount > 0)
		interactionWidget->ShowInViewport(VO_INTERACTION);

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}

void ADAPC_InGame::CloseLoadingScreen()
{
	loadingWidget->RemoveInViewport();
	SetInputMode(FInputModeGameOnly());
	auto level = Cast<ADA_LevelScript>(GetWorld()->GetLevelScriptActor());
	if (level != nullptr && level->IsAutoStart())
		level->AdmissionPlayer(this);
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}
