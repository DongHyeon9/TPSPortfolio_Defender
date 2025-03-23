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
	//Name of player using the store
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_Info_PlayerName;

	//Widget indicating store time limit
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_Timer;

	//Gold held by the player
	UPROPERTY(BlueprintReadOnly, category = "Shop", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_GoldHas;

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

	//모든 웨펀 슬롯을 배열로 저장해 놓는다
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Shop", meta = (AllowPrivateAccess = true))
	TArray<UDWP_WeaponSlot*> weaponSlots;

	//모든 포션 슬롯을 배열로 저장해 놓는다
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Shop", meta = (AllowPrivateAccess = true))
	TArray<UDWP_PotionSlot*> potionSlots;

	//모든 아이템 슬롯을 배열로 저장해 놓는다
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Shop", meta = (AllowPrivateAccess = true))
	TArray<UDWP_ItemSlot*> itemSlots;

	UPROPERTY()
	UDWP_BuySellButton*			buySellButtonWidget;	//생성된 구매와 판매 버튼의 위젯

	UPROPERTY()
	UDWP_Count*					countWidget;			//생성된 갯수 버튼의 위젯

	FString						selectedItemName;		//선택된 아이템의 이름
	EITEM_TYPE					selectedItemType;		//선택된 아이템의 타입
	UDC_Inventory*				inventoryComp;			//플레이어의 인벤토리 컴포넌트
	UDC_Weapon*					weaponComp;				//플레이어의 웨펀 컴포넌트
	TArray<FDFItem*>			itemDataArray;			//플레이어의 아이템 데이터 배열

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
