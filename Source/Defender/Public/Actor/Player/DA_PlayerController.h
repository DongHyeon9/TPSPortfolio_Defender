#pragma once

#include "../Defender.h"
#include "GameFramework/PlayerController.h"
#include "DA_PlayerController.generated.h"

class UDW_PlayerWidget;

//������Ʈ���� ���Ǵ� �÷��̾���Ʈ�ѷ��� �ֻ��� Ŭ����
UCLASS(Abstract, NotBlueprintable)
class DEFENDER_API ADA_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:

protected:
	UPROPERTY()
	UDW_PlayerWidget*				loadingWidget;				//�ε� ������ �ν��Ͻ�

private:
	//Loading widget class to display on the loading screen
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Widget", meta = (AllowPrivateAccess = true))
	TSubclassOf<UDW_PlayerWidget> loadingWidgetClass;

public:
	ADA_PlayerController();
	/*
	* Convert player data to json format.
	* @return Retruns FString
	*/
	virtual FString SaveJson();
	/*
	* Saves player data for the current level, opens a loading window, and opens the next level.
	* @param[in] _Level the level to open
	* @param[in] _bAbsolute if true options are reset, if false options are carried over from current level
	* @param[in] _Options a string of options to use for the travel URL
	*/
	UFUNCTION(BlueprintCallable, category = "LevelMove")
	void OpenLevel(FName _Level, bool _bAbsolute = true, FString _Options = FString(TEXT("")));

protected:
	virtual void BeginPlay()override;

private:

};
