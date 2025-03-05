#pragma once

#include "Widget/DW_PlayerWidget.h"
#include "DWP_Infomation.generated.h"

class UDWP_WeaponSlot;
class UDWP_PotionSlot;
class UDWP_EquipDropButton;

class UTextBlock;
class UImage;

//종합적인 캐릭터의 정보를 상세히 담고있는 위젯
UCLASS()
class DEFENDER_API UDWP_Infomation : public UDW_PlayerWidget
{
	GENERATED_BODY()
public:

protected:

private:
	//Background image of information window
	UPROPERTY(BlueprintReadOnly, category = "Information", meta = (BindWidget, AllowPrivateAccess = true))
	UImage* img_Background;

	//player's name
	UPROPERTY(BlueprintReadOnly, category = "Information", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_Name;

	//player's level
	UPROPERTY(BlueprintReadOnly, category = "Information", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_Level;

	//player's current HP
	UPROPERTY(BlueprintReadOnly, category = "Information", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_HPCurrent;

	//player's max HP
	UPROPERTY(BlueprintReadOnly, category = "Information", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_HPMax;

	//player's default damage
	UPROPERTY(BlueprintReadOnly, category = "Information", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_DamageBase;

	//player's additional damage
	UPROPERTY(BlueprintReadOnly, category = "Information", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_DamageAdd;

	//player's default speed
	UPROPERTY(BlueprintReadOnly, category = "Information", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_SpeedBase;

	//player's  additional speed
	UPROPERTY(BlueprintReadOnly, category = "Information", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_SpeedAdd;

	//player's current EXP
	UPROPERTY(BlueprintReadOnly, category = "Information", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_EXPCurrent;

	//Amount of experience required to reach different levels
	UPROPERTY(BlueprintReadOnly, category = "Information", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_EXPTotal;

	//gold the player has
	UPROPERTY(BlueprintReadOnly, category = "Information", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_GoldHas;

	//player's current score
	UPROPERTY(BlueprintReadOnly, category = "Information", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_ScoreCurrent;

	//player's best score
	UPROPERTY(BlueprintReadOnly, category = "Information", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_ScoreBest;

	//Widget to display weapons
	UPROPERTY(BlueprintReadOnly, category = "Weapon Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_WeaponSlot* WG_WeaponSlot_0;

	//Widget to display weapons
	UPROPERTY(BlueprintReadOnly, category = "Weapon Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_WeaponSlot* WG_WeaponSlot_1;

	//Widget to display weapons
	UPROPERTY(BlueprintReadOnly, category = "Weapon Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_WeaponSlot* WG_WeaponSlot_2;

	//Widget to display weapons
	UPROPERTY(BlueprintReadOnly, category = "Weapon Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_WeaponSlot* WG_WeaponSlot_3;

	//Widget to display weapons
	UPROPERTY(BlueprintReadOnly, category = "Weapon Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_WeaponSlot* WG_WeaponSlot_4;

	//Widget to display weapons
	UPROPERTY(BlueprintReadOnly, category = "Weapon Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_WeaponSlot* WG_WeaponSlot_5;

	//Widget to display weapons
	UPROPERTY(BlueprintReadOnly, category = "Weapon Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_WeaponSlot* WG_WeaponSlot_6;

	//Widget to display weapons
	UPROPERTY(BlueprintReadOnly, category = "Weapon Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_WeaponSlot* WG_WeaponSlot_7;

	//Widget to display weapons
	UPROPERTY(BlueprintReadOnly, category = "Weapon Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_WeaponSlot* WG_WeaponSlot_8;

	//Widget to display weapons
	UPROPERTY(BlueprintReadOnly, category = "Weapon Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_WeaponSlot* WG_WeaponSlot_9;

	//Widget to display weapons
	UPROPERTY(BlueprintReadOnly, category = "Weapon Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_WeaponSlot* WG_WeaponSlot_10;

	//Widget to display weapons
	UPROPERTY(BlueprintReadOnly, category = "Weapon Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_WeaponSlot* WG_WeaponSlot_11;

	//Widget to display potion
	UPROPERTY(BlueprintReadOnly, category = "Potion Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_PotionSlot* WG_PotionSlot_0;

	//Widget to display potion
	UPROPERTY(BlueprintReadOnly, category = "Potion Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_PotionSlot* WG_PotionSlot_1;

	//Widget to display potion
	UPROPERTY(BlueprintReadOnly, category = "Potion Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_PotionSlot* WG_PotionSlot_2;

	//Widget to display potion
	UPROPERTY(BlueprintReadOnly, category = "Potion Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_PotionSlot* WG_PotionSlot_3;

	//Widget to display potion
	UPROPERTY(BlueprintReadOnly, category = "Potion Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_PotionSlot* WG_PotionSlot_4;

	//Widget to display potion
	UPROPERTY(BlueprintReadOnly, category = "Potion Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_PotionSlot* WG_PotionSlot_5;

	//Equip or drop widget that appears when you press the potion widget or weapon widget
	UPROPERTY(BlueprintReadWrite, category = "Equip Drop Button", meta = (AllowPrivateAccess = true))
	UDWP_EquipDropButton* equipDropButtonWidget;

	//Equip or drop widget class
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Equip Drop Button", meta = (AllowPrivateAccess))
	TSubclassOf<UDWP_EquipDropButton> equipDropButtonWidgetClass;

	//Weapon Data Table
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "WaeponData", meta = (AllowPrivateAccess))
	UDataTable* weaponDataTable;

	//Potion Data Table
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "PotionData", meta = (AllowPrivateAccess))
	UDataTable* itemDataTable;

	TArray<UDWP_WeaponSlot*>		weaponSlots{};		//무기 슬롯 위젯의 배열
	TArray<UDWP_PotionSlot*>		potionSlots{};		//포션 슬롯 위젯의 배열

public:
	UDWP_Infomation(const FObjectInitializer& _ObjectInitializer);
	/*
	* Initialize widgets before adding them to the viewport.
	*/
	virtual void InitializeWidget()override;

protected:
	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;
	virtual FReply NativeOnKeyDown(const FGeometry& _InGeometry, const FKeyEvent& _InKeyEvent)override;

private:
	//내부 호출 함수

	UFUNCTION()
	void OnClicked(const FString& _ItemName, EITEM_TYPE _ItemType);

	UFUNCTION()
	void SetEXP(int32 _TotalEXP, int32 _CurrentEXP);

	UFUNCTION()
	void SetLevel(int32 _Level);

	UFUNCTION()
	void SetGold(int32 _TotalGold);

	UFUNCTION()
	void SetHP(float _MaxHP, float _CurrentHP);

	UFUNCTION()
	void SetSpeed(float _BaseSpeed, float _AdditionalSpeed);

	UFUNCTION()
	void SetDamage(float _BaseDamage, float _AdditionalDamage);

	UFUNCTION()
	void SetScore(int32 _Current, int32 _High);

	UFUNCTION()
	void SetPlayerName(const FString& _Name);
};
