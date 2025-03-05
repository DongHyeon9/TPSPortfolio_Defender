#pragma once

#include "Widget/DW_PlayerWidget.h"
#include "DWP_PotionSlot.generated.h"

class UTextBlock;
class UImage;
class UButton;

struct FPotionSlotInitializer;

//�÷��̾ �������� ������ ǥ���ϴ� ����
UCLASS()
class DEFENDER_API UDWP_PotionSlot : public UDW_PlayerWidget
{
	GENERATED_BODY()
public:
	//Delegate called when btn_Potion is clicked
	UPROPERTY(BlueprintAssignable)
	FOnButtonClickedDelegate onButtonClicked;

protected:

private:
	//Widget that displays an image of a potion
	UPROPERTY(BlueprintReadOnly, category = "Potion Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UImage* img_Potion;

	//Widget that displays the current number of potions
	UPROPERTY(BlueprintReadOnly, category = "Potion Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_Count;

	//Button indicating whether the current widget has been clicked
	UPROPERTY(BlueprintReadOnly, category = "Potion Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UButton* btn_Potion;

	//Widget that displays the description of the potion
	UPROPERTY(BlueprintReadOnly, category = "Potion Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_Description;

	//Name of current potion
	UPROPERTY(BlueprintReadOnly, category = "itemState", meta = (AllowPrivateAccess = true))
	FString potionName;

public:
	UDWP_PotionSlot(const FObjectInitializer& _ObjectInitializer);

	/*
	* Initialize potion slot widgets
	* @param[in] _Initializer potion data
	*/
	void InitializePotionSlot(const FPotionSlotInitializer& _Initializer);
	/*
	* Sets the name of the potion
	* @param[in] _PotionName Name of potion to set
	*/
	void SetPotionName(const FString& _PotionName);
	/*
	* Set the image of the potion
	* @param[in] _Image Image to set for potion
	*/
	void SetPotionImage(UTexture2D* _Image);
	/*
	* Set the description of the potion
	* @param[in] _Description Description of potion to set
	*/
	void SetPotionDescription(const FText& _Description);
	/*
	* Returns the name of the current potion
	* @return returns const FString&
	*/
	FORCEINLINE const FString& GetItemName()const { return potionName; }


protected:
	virtual void NativeDestruct()override;

private:
	//���� ȣ�� �Լ�

	UFUNCTION()
	void OnHovered();

	UFUNCTION()
	void OnUnHovered();

	UFUNCTION()
	void OnCliecked();

	UFUNCTION()
	void SetItemCount(const FString& _ItemName, int32 _TotalItemCount);
};

struct FPotionSlotInitializer {
	FPotionSlotInitializer() :
		itemData(),
		currentCount()
	{}

	//�������� ������
	//*�ʱ�ȭ ������ ������ ����� �������� ����*
	FDFItem* itemData;
	//������ ���� ����
	int32 currentCount;
};