#pragma once

#include "Widget/DW_PlayerWidget.h"
#include "DWP_ItemSlot.generated.h"

class UTextBlock;
class UButton;
class UImage;

struct FItemSlotInitilizer;
struct FMargin;

//�������� �������� ǥ���ϴ� ����(�Ҹ�ǰ,����)
UCLASS()
class DEFENDER_API UDWP_ItemSlot : public UDW_PlayerWidget
{
	GENERATED_BODY()
public:
	//Delegate called when btn_Item is clicked
	UPROPERTY(BlueprintAssignable)
	FOnButtonClickedDelegate onButtonClicked;

protected:

private:
	//Display the image of the item
	UPROPERTY(BlueprintReadOnly, category = "ItemSlot", meta = (BindWidget, AllowPrivateAccess = true))
	UImage* img_Item;

	//Display the name of the item
	UPROPERTY(BlueprintReadOnly, category = "ItemSlot", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_ItemName;

	//Display the price of the item
	UPROPERTY(BlueprintReadOnly, category = "ItemSlot", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_Price;

	//Button to select an item
	UPROPERTY(BlueprintReadOnly, category = "ItemSlot", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* btn_Item;

	//Weapon Data Table
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "WeaponData", meta = (AllowPrivateAccess = true))
	UDataTable* weaponDataTable;

	FString					itemName;					//���� ������ �������� �̸�
	EITEM_TYPE				currentItemType;			//�������� Ÿ��
	EWEAPON_OWNED_STATE		currentWeaponOwnedState;	//������ ��� ���� ����

public:
	UDWP_ItemSlot(const FObjectInitializer& _ObjectInitializer);
	/*
	* Initialize item slots.
	* You can select either potion data or weapon data to initialize it.
	* @param[in] _Initializer Initialization data of the item.
	*/
	void InitializeItemSlot(const FItemSlotInitilizer& _Initializer);

protected:
	virtual void NativeDestruct()override;

private:
	UFUNCTION()
	void OnClicked();

	UFUNCTION()
	void OnWeaponChangeStated(const FString& _WeaponName, EWEAPON_OWNED_STATE _WeaponOwnedState);
};

struct FItemSlotInitilizer {
	FItemSlotInitilizer() :
		potionData(),
		weaponData(),
		inPadding()
	{}

	//���������� ��� �ʱ�ȭ ���ش�
	FDFItem* potionData;

	//���� ������ ��� �ʱ�ȭ ���ش�
	FWeaponOwnedData* weaponData;

	//�̹����� ����
	FMargin inPadding;
};