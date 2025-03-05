#pragma once

#include "Widget/DW_PlayerWidget.h"
#include "DWP_ItemSlot.generated.h"

class UTextBlock;
class UButton;
class UImage;

struct FItemSlotInitilizer;
struct FMargin;

//상점에서 아이템을 표시하는 위젯(소모품,무기)
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

	FString					itemName;					//현재 설정된 아이템의 이름
	EITEM_TYPE				currentItemType;			//아이템의 타입
	EWEAPON_OWNED_STATE		currentWeaponOwnedState;	//무기일 경우 소유 상태

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

	//포션종류일 경우 초기화 해준다
	FDFItem* potionData;

	//무기 종류일 경우 초기화 해준다
	FWeaponOwnedData* weaponData;

	//이미지의 여백
	FMargin inPadding;
};