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

//�������� ��� �� �� �ִ� ����
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

	//��� ���� ������ �迭�� ������ ���´�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Shop", meta = (AllowPrivateAccess = true))
	TArray<UDWP_WeaponSlot*> weaponSlots;

	//��� ���� ������ �迭�� ������ ���´�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Shop", meta = (AllowPrivateAccess = true))
	TArray<UDWP_PotionSlot*> potionSlots;

	//��� ������ ������ �迭�� ������ ���´�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Shop", meta = (AllowPrivateAccess = true))
	TArray<UDWP_ItemSlot*> itemSlots;

	UPROPERTY()
	UDWP_BuySellButton*			buySellButtonWidget;	//������ ���ſ� �Ǹ� ��ư�� ����

	UPROPERTY()
	UDWP_Count*					countWidget;			//������ ���� ��ư�� ����

	FString						selectedItemName;		//���õ� �������� �̸�
	EITEM_TYPE					selectedItemType;		//���õ� �������� Ÿ��
	UDC_Inventory*				inventoryComp;			//�÷��̾��� �κ��丮 ������Ʈ
	UDC_Weapon*					weaponComp;				//�÷��̾��� ���� ������Ʈ
	TArray<FDFItem*>			itemDataArray;			//�÷��̾��� ������ ������ �迭

	FTimerHandle				shopTimerHandle;		//������ ���ѽð��� �����ϴ� Ÿ�̸� �ڵ�
	float						timeRemaining;			//������ �����ð��� ��Ÿ���ϴ�

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
