#include "Actor/Player/DA_PlayerController.h"
#include "Widget/DW_PlayerWidget.h"
#include "Server/DA_GameInstance.h"

#include "Kismet/GameplayStatics.h"

ADA_PlayerController::ADA_PlayerController()
{
	
}

FString ADA_PlayerController::SaveJson()
{
	DFLOG_N(Display);
	return FString();
}

void ADA_PlayerController::OpenLevel(FName _Level, bool _bAbsolute, FString _Options)
{
	DFLOG_N(Display);

	//�ε������� �����ϰ���ϴ�
	DFCHECK(loadingWidget != nullptr &&
		loadingWidget->ShowInViewport(VO_LOADING_SCREEN));

	SetInputMode(FInputModeUIOnly());

	//���� ���� �����͸� �����ν��Ͻ��� �����մϴ�
	auto gameInstance = GetGameInstance<UDA_GameInstance>();
	DFCHECK(gameInstance != nullptr);
	gameInstance->SaveCurrentLevelData(SaveJson());
	gameInstance->ResetTimer();

	if (loadingWidget->IsInViewport())
		SetInputMode(FInputModeGameOnly());

	UGameplayStatics::OpenLevel(this, _Level, _bAbsolute, _Options);
}

void ADA_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	DFLOG_N(Display);

	loadingWidget = CreateWidget<UDW_PlayerWidget>(this, loadingWidgetClass);
}
