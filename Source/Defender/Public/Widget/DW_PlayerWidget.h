#pragma once

#include "Widget/DW_UserWidget.h"
#include "DW_PlayerWidget.generated.h"

class ADA_PlayerController;
class ADA_Player;
class ADA_PlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonClickedDelegate, const FString&, _ItemName, EITEM_TYPE, _ItemType);

//플레이가 사용할 위젯의 최상위 클래스
UCLASS()
class DEFENDER_API UDW_PlayerWidget : public UDW_UserWidget
{
	GENERATED_BODY()

public:

protected:

private:
	//Player controller owns the widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Owner", meta = (AllowPrivateAccess = true))
	ADA_PlayerController* ownerController;

	//Player state owns the widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Owner", meta = (AllowPrivateAccess = true))
	ADA_PlayerState* ownerState;

	//Player character owns the widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Owner", meta = (AllowPrivateAccess = true))
	ADA_Player* ownerPlayer;

public:
	UDW_PlayerWidget(const FObjectInitializer& _ObjectInitializer);

	/*
	* Returns the player controller that owns the UI
	* @return Returns ADA_PlayerController*
	*/
	FORCEINLINE ADA_PlayerController* GetOwnerPlayerController()const { return ownerController; }
	
	/*
	* Gets the player character controlled by the player controller.
	* @return Returns ADA_Player*
	*/
	FORCEINLINE ADA_Player* GetOwnerPlayerCharacter()const { return ownerPlayer; }

	/*
	* Gets the player state of the current player
	* @return Returns ADA_PlayerState*
	*/
	FORCEINLINE ADA_PlayerState* GetOwnerPlayerState()const { return ownerState; }
protected:
	virtual void NativeConstruct()override;

	/*
	* Converts the number of bullets in the current weapon into text.
	* @param[in] _WeaponOwnedData Current weapon data
	* @return returns FText
	*/
	FText SetMagazineText(const FWeaponOwnedData& _WeaponOwnedData);
	/*
	* Converts the amount of gold into text to display on the UI
	* @param[in] _Gold amount of gold
	* @return returns FText
	*/
	FText SetGoldText(int32 _Gold);
	/*
	* Change the time according to the format.
	* @param[in] _Time Time to change to text
	* @return The text is returned in the form minutes:seconds
	*/
	FText SetTimeText(int32 _Time);
private:

};
