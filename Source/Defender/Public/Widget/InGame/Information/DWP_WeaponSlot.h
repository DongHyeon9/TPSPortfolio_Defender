#pragma once

#include "Widget/DW_PlayerWidget.h"
#include "DWP_WeaponSlot.generated.h"

class UTextBlock;
class UImage;
class UButton;

//플레이어가 보유중인 무기,총알갯수를 표시하는 위젯
UCLASS()
class DEFENDER_API UDWP_WeaponSlot : public UDW_PlayerWidget
{
	GENERATED_BODY()
public:
	//Delegate called when btn_Item is clicked
	UPROPERTY(BlueprintAssignable)
	FOnButtonClickedDelegate onButtonClicked;

protected:

private:
	//Image widget to display current weapon possession status
	UPROPERTY(BlueprintReadOnly, category = "Weapon Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UImage* img_Enable;

	//Image of current weapon
	UPROPERTY(BlueprintReadOnly, category = "Weapon Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UImage* img_Weapon;

	//Name of current weapon
	UPROPERTY(BlueprintReadOnly, category = "Weapon Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_WeaponName;

	//Current weapon's magazine capacity
	UPROPERTY(BlueprintReadOnly, category = "Weapon Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_Magazine;

	//Button to determine whether the widget was clicked
	UPROPERTY(BlueprintReadOnly, category = "Weapon Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* btn_Item;

	//The name of the weapon currently displayed by the widget
	UPROPERTY(BlueprintReadOnly, category = "itemState", meta = (AllowPrivateAccess = true))
	FString weaponName;

	//Current status of weapons
	UPROPERTY(BlueprintReadOnly, category = "itemState", meta = (AllowPrivateAccess = true))
	EWEAPON_OWNED_STATE currentWeaponOwnedState;

public:
	UDWP_WeaponSlot(const FObjectInitializer& _ObjectInitializer);

	/*
	* Initialize waepon slot widgets
	* @param[in] _Initializer weapon data
	*/
	void InitializeWeaponSlot(const FWeaponOwnedData& _Initializer);
	/*
	* img_Enable is modified depending on the current weapon possession status.
	* @param[in] _WeaponOwnedState Current status of weapons
	*/
	void SetWeaponOwnedState(EWEAPON_OWNED_STATE _WeaponOwnedState);
	/*
	* Sets the name of the current weapon
	* @param[in] _ItemName name to be set
	*/
	void SetWeaponName(const FString& _WeaponName);
	/*
	* Returns the name of the current weapon
	* @return returns const FString&
	*/
	FORCEINLINE const FString& GetItemName()const { return weaponName; }
	/*
	* Sets the current number of magazines.
	* @param[in] _WeaponData Data for the weapon whose magazine you want to change
	*/
	UFUNCTION()
	void SetMagazine(const FWeaponOwnedData& _WeaponData);
	/*
	* Set the possession status of the weapon
	* @param[in] _WeaponName Name of the set weapon
	* @param[in] _WeaponOwnedState Set state of the weapon
	*/
	UFUNCTION()
	void SetWeaponState(const FString& _WeaponName, EWEAPON_OWNED_STATE _WeaponOwnedState);

protected:
	virtual void NativeDestruct()override;

private:
	//내부 호출 함수

	UFUNCTION()
	void OnCliecked();
};