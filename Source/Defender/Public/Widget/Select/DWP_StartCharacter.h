#pragma once

#include "Widget/DW_PlayerWidget.h"
#include "DWP_StartCharacter.generated.h"

class UDWP_CharacterSelectSlot;
class ADAPC_Select;

class UButton;
class UEditableTextBox;

//ĳ���͸� �����ϴ� �ܰ��� ����
UCLASS()
class DEFENDER_API UDWP_StartCharacter : public UDW_PlayerWidget
{
	GENERATED_BODY()
public:

protected:

private:
	//Widget slot that contains character information and can be selected
	UPROPERTY(BlueprintReadOnly, category = "Start Character", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_CharacterSelectSlot* WG_CharacterSelectSlot_1;

	//Widget slot that contains character information and can be selected
	UPROPERTY(BlueprintReadOnly, category = "Start Character", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_CharacterSelectSlot* WG_CharacterSelectSlot_2;

	//Widget slot that contains character information and can be selected
	UPROPERTY(BlueprintReadOnly, category = "Start Character", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_CharacterSelectSlot* WG_CharacterSelectSlot_3;

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
	ADAPC_Select*							ownerPlayerController;			//������ �������ִ� �÷��̾� ��Ʈ�ѷ�
	int32									selectedIndex;					//������ ĳ������ �ε���
	TArray<UDWP_CharacterSelectSlot*>		characterSlots;					//ĳ���ͼ��� ���������� ����ִ� �迭

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
