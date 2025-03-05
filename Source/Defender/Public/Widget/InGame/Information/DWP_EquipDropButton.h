#pragma once

#include "Widget/DW_PlayerWidget.h"
#include "DWP_EquipDropButton.generated.h"

class UButton;

//아이템을 버릴지 장착할지 선택하는 위젯
UCLASS()
class DEFENDER_API UDWP_EquipDropButton : public UDW_PlayerWidget
{
	GENERATED_BODY()
public:

protected:

private:
	//Button to select equip
	UPROPERTY(BlueprintReadOnly, category = "Button", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* btn_Equip;

	//Button to select drop
	UPROPERTY(BlueprintReadOnly, category = "Button", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* btn_Drop;

	//Name of currently selected item
	UPROPERTY(BlueprintReadOnly, category = "Item Info", meta = (AllowPrivateAccess = true))
	FString itemName;

	//Type of currently selected item
	UPROPERTY(BlueprintReadOnly, category = "Item Info", meta = (AllowPrivateAccess = true))
	EITEM_TYPE currentItemType;

public:
	UDWP_EquipDropButton(const FObjectInitializer& _ObjectInitializer);
	/*
	* Set the name of the current item
	* @param[in] _ItemName Name to set
	*/
	void SetItemName(const FString& _ItemName);
	/*
	* Set the type of the current item
	* @param[in] _ItemType Type to set
	*/
	void SetItemType(EITEM_TYPE _ItemType);
	/*
	* Get the equip button
	* @return Retuns UButton*
	*/
	FORCEINLINE UButton* GetEquipButton()const { return btn_Equip; }
	/*
	* Get the drop button
	* @return Retuns UButton*
	*/
	FORCEINLINE UButton* GetDropButton()const { return btn_Drop; }

protected:
	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;

private:
	//내부호출 함수

	UFUNCTION(BlueprintCallable, category = "EquipDropButton")
	void OnClickedDrop();

	UFUNCTION(BlueprintCallable, category = "EquipDropButton")
	void OnClickedEquip();
};
