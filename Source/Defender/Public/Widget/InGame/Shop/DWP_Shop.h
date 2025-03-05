#pragma once

#include "Widget/DW_PlayerWidget.h"
#include "DWP_Shop.generated.h"

class UDWP_ItemSlot;
class UDWP_WeaponSlot;
class UDWP_PotionSlot;
class UDWP_BuySellButton;
class UDWP_Count;
class UDC_Inventory;
class UDC_Weapon;

class UTextBlock;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseShopDelegate);

//아이템을 사고 팔 수 있는 위젯
UCLASS()
class DEFENDER_API UDWP_Shop : public UDW_PlayerWidget
{
	GENERATED_BODY()
public:
	//Delegate called when the store is closed
	UPROPERTY(BlueprintAssignable)
	FOnCloseShopDelegate onClose;

protected:

private:
	//first item slot
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_ItemSlot* WG_ItemSlot_0;

	//second item slot
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_ItemSlot* WG_ItemSlot_1;

	//Third item slot
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_ItemSlot* WG_ItemSlot_2;

	//fourth item slot
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_ItemSlot* WG_ItemSlot_3;

	//Fifth item slot
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_ItemSlot* WG_ItemSlot_4;

	//Sixth item slot
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_ItemSlot* WG_ItemSlot_5;

	//Seventh item slot
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_ItemSlot* WG_ItemSlot_6;

	//Eighth item slot
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_ItemSlot* WG_ItemSlot_7;

	//Ninth item slot
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_ItemSlot* WG_ItemSlot_8;

	//Tenth item slot
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_ItemSlot* WG_ItemSlot_9;

	//Eleventh item slot
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_ItemSlot* WG_ItemSlot_10;

	//Twelfth item slot
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_ItemSlot* WG_ItemSlot_11;

	//Thirteenth item slot
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_ItemSlot* WG_ItemSlot_12;

	//Fourteenth item slot
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_ItemSlot* WG_ItemSlot_13;

	//Fifteenth item slot
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_ItemSlot* WG_ItemSlot_14;

	//Sixteenth item slot
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_ItemSlot* WG_ItemSlot_15;

	//Seventeenth item slot
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_ItemSlot* WG_ItemSlot_16;

	//Name of player using the store
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_Info_PlayerName;

	//Widget indicating store time limit
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_Timer;

	//Gold held by the player
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_GoldHas;

	//The first weapon the player owns
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_WeaponSlot* WG_WeaponSlot_Info_0;

	//The player's second weapon
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_WeaponSlot* WG_WeaponSlot_Info_1;

	//Third weapon owned by the player
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_WeaponSlot* WG_WeaponSlot_Info_2;

	//The fourth weapon owned by the player
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_WeaponSlot* WG_WeaponSlot_Info_3;

	//The player's fifth weapon
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_WeaponSlot* WG_WeaponSlot_Info_4;

	//The sixth weapon owned by the player
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_WeaponSlot* WG_WeaponSlot_Info_5;

	//The seventh weapon owned by the player
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_WeaponSlot* WG_WeaponSlot_Info_6;

	//The eighth weapon owned by the player
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_WeaponSlot* WG_WeaponSlot_Info_7;

	//The player's ninth weapon
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_WeaponSlot* WG_WeaponSlot_Info_8;

	//The player's tenth weapon
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_WeaponSlot* WG_WeaponSlot_Info_9;

	//The eleventh weapon owned by the player
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_WeaponSlot* WG_WeaponSlot_Info_10;

	//The first potion held by the player
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_PotionSlot* WG_PotionSlot_Info_0;

	//The second potion held by the player
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_PotionSlot* WG_PotionSlot_Info_1;

	//Third potion held by the player
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_PotionSlot* WG_PotionSlot_Info_2;

	//The fourth potion held by the player
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_PotionSlot* WG_PotionSlot_Info_3;

	//The fifth potion held by the player
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_PotionSlot* WG_PotionSlot_Info_4;

	//The sixth potion held by the player
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UDWP_PotionSlot* WG_PotionSlot_Info_5;

	//Button to close the store
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* btn_Close;

	//Potion Data Table
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "PotionData", meta = (AllowPrivateAccess = true))
	UDataTable* itemDataTable;

	//Widget class for buy or sell button
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "WidgetData", meta = (AllowPrivateAccess = true))
	TSubclassOf<UDWP_BuySellButton> buySellButtonClass;

	//A class of widgets that will display the number of purchases and sales.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "WidgetData", meta = (AllowPrivateAccess = true))
	TSubclassOf<UDWP_Count> countClass;

	UPROPERTY()
	UDWP_BuySellButton*			buySellButtonWidget;	//생성된 구매와 판매 버튼의 위젯

	UPROPERTY()
	UDWP_Count*					countWidget;			//생성된 갯수 버튼의 위젯

	FString						selectedItemName;		//선택된 아이템의 이름
	EITEM_TYPE					selectedItemType;		//선택된 아이템의 타입
	UDC_Inventory*				inventoryComp;			//플레이어의 인벤토리 컴포넌트
	UDC_Weapon*					weaponComp;				//플레이어의 웨펀 컴포넌트
	TArray<FDFItem*>			itemDataArray;			//플레이어의 아이템 데이터 배열

	TArray<UDWP_WeaponSlot*>	weaponSlots;			//모든 웨펀 슬롯을 배열로 저장해 놓는다
	TArray<UDWP_PotionSlot*>	potionSlots;			//모든 포션 슬롯을 배열로 저장해 놓는다
	TArray<UDWP_ItemSlot*>		itemSlots;				//모든 아이템 슬롯을 배열로 저장해 놓는다
	FTimerHandle				shopTimerHandle;		//상점의 제한시간을 관리하는 타이머 핸들
	float						timeRemaining;			//상점의 남은시간을 나타냅니다

public:
	/*
	* Initialize widgets before adding them to the viewport.
	*/
	virtual void InitializeWidget()override;
	/*
	* close the store
	*/
	UFUNCTION()
	void OnClickedClose();
	/*
	* Set a time limit for the store.
	* 0.f pause timer does not appear
	* @param[in] _Timer time to set
	*/
	void SetTimer(float _Timer);

protected:
	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;

private:
	UFUNCTION()
	void OnClicked(const FString& _ItemName, EITEM_TYPE _ItemType);

	UFUNCTION()
	void OnClickedTradeButton(bool _bIsBuy);

	UFUNCTION()
	void Trade(int32 _Count, int32 _TotalPrice);

	UFUNCTION()
	void SetGold(int32 _Gold);

	UFUNCTION()
	void ResetPotionSlots(const FString& _PotionName, int32 _Count);

	UFUNCTION()
	void SetPlayerName(const FString& _Name);

	void SetPotionSlot();
};
