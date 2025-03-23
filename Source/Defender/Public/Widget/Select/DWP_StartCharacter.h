#pragma once

#include "Widget/DW_PlayerWidget.h"
#include "DWP_StartCharacter.generated.h"

class UDWP_CharacterSelectSlot;
class ADAPC_Select;

class UButton;
class UEditableTextBox;

//캐릭터를 선택하는 단계의 위젯
UCLASS()
class DEFENDER_API UDWP_StartCharacter : public UDW_PlayerWidget
{
	GENERATED_BODY()
public:

protected:

private:
	//캐릭터선택 위젯슬롯을 담고있는 배열
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Start Character", meta = (AllowPrivateAccess = true))
	TArray<UDWP_CharacterSelectSlot*> characterSlots;

	//Button to check if you can go to the skill selection screen when clicked
	UPROPERTY(BlueprintReadOnly, category = "Start Character", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* btn_Check;

	//Button to return to main screen when clicked
	UPROPERTY(BlueprintReadOnly, category = "Start Character", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* btn_Back;

	//Nickname the player will use in game
	UPROPERTY(BlueprintReadOnly, category = "Start Character", meta = (BindWidget, AllowPrivateAccess = true))
	UEditableTextBox* etb_ID;

	//Data table containing information on characters to be assigned to widget slots
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Start Character Setting", meta = (AllowPrivateAccess = true))
	UDataTable* characterDataTable;

	UPROPERTY()
	ADAPC_Select*							ownerPlayerController;			//위젯을 가지고있는 플레이어 컨트롤러
	int32									selectedIndex;					//선택한 캐릭터의 인덱스

public:
	UDWP_StartCharacter(const FObjectInitializer& _ObjectInitializer);
	/*
	* Initialize widgets before adding them to the viewport.
	*/
	virtual void InitializeWidget()override;

protected:
	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;

private:
	UFUNCTION()
	void OnClickedCheck();

	UFUNCTION()
	void OnChecked(UDWP_CharacterSelectSlot* _CheckedSlot);
};
